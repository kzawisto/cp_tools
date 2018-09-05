
#pragma once


#include<boost/date_time.hpp>
#include<boost/date_time/local_time/local_date_time.hpp>
#include<iostream>
#include<boost/date_time/posix_time/posix_time.hpp>
namespace cp_tools {
namespace gr= boost::gregorian;
namespace pt= boost::posix_time;
namespace lt= boost::local_time;

struct Date {
	int year, month, day;
	friend bool operator<(Date a, Date b){
		return (a.year < b.year) or (a.year == b.year && a.month < b.month) or
			(a.year == b.year && a.month == b.month && a.day < b.day);
	}
	friend bool operator==(Date a, Date b){
			return (a.year == b.year && a.month == b.month && a.day == b.day);
	}
};

struct Time {
	int hour, min;
	double sec;

	friend bool operator<(Time a, Time b){
		return (a.hour < b.hour) or (a.hour == b.hour && a.min < b.min) or
			(a.hour == b.hour && a.min == b.min && a.sec < b.sec-1e-8);
	}
	friend bool operator==(Time a, Time b){
		return
			(a.hour == b.hour && a.min == b.min && abs(a.sec - b.sec)<1e-8);
	}
};

struct DateTime {
	Date date; Time time;


	friend bool operator<(const DateTime & a, const DateTime & b){
			return (a.date < b.date) or (a.date == b.date && a.time < b.time);
	}
	friend bool operator==(const DateTime & a, const DateTime & b){
			return (a.date == b.date) and (a.time == b.time);
	}
	friend std::ostream & operator<<(std::ostream & o, const DateTime &b) {
		o<<b.date.year<<"-"<<b.date.month<<"-"<<b.date.day<<" "
				<< b.time.hour<<":"<<b.time.min<<":"<<b.time.sec;
		return o;
	}
};

DateTime date_time_from_ptime(pt::ptime t) {
		DateTime dt;
		dt.date.day = t.date().day();
		dt.date.month = t.date().month();
		dt.date.year = t.date().year();
		dt.time.hour = t.time_of_day().hours();
		dt.time.min = t.time_of_day().minutes();
		dt.time.sec = t.time_of_day().seconds();
		return dt;
}

struct MonthId {
	long double epoch;
	DateTime date;
	friend bool operator<(const MonthId & a, const MonthId & b) {
		return a.epoch < b.epoch;
	}
};

lt::local_date_time build_local_time(
		gr::date dt, pt::time_duration time,
	boost::shared_ptr< boost::local_time::posix_time_zone > tz
		) {
	return lt::local_date_time(dt,
						 time, tz, lt::local_date_time::check_dst(dt, time, tz));
}
struct TimeToEpochConverter {
	std::vector<MonthId> key_frames;
	boost::shared_ptr< boost::local_time::posix_time_zone > tz;

	std::vector<MonthId> get_month_frames(int year) {
		std::vector<MonthId> cache;
		for(int j =1; j <= 12;++j) {


		long double epoch = to_time_t(build_local_time(gr::date(year,j,1),
						 pt::time_duration(0,0,0), tz).utc_time());
			cache.push_back(MonthId {
				epoch, DateTime{Date{year, j, 1},Time{0,0,0}}});
		}
		return cache;
	}
	std::vector<MonthId> get_dst_frames(int year) {
			auto start_dst_local_time = tz->dst_local_start_time(year) + tz->dst_offset();
			auto start_dst_utc_time =
				lt::local_date_time(
					start_dst_local_time.date(), start_dst_local_time.time_of_day(),
					tz, true
			).utc_time();
			auto end_dst_local_time = tz->dst_local_end_time(year);
			auto end_dst_utc_time = lt::local_date_time(
					end_dst_local_time.date(), end_dst_local_time.time_of_day(),
					tz, false
			).utc_time();

			return std::vector<MonthId>{
				MonthId{ to_time_t(start_dst_utc_time), date_time_from_ptime(start_dst_local_time)},
				MonthId{ to_time_t(end_dst_utc_time), date_time_from_ptime(end_dst_local_time)}
			};
	}
	std::vector<MonthId> get_frames_per_year(int year) {
			auto frames_month = get_month_frames(year);
			if(tz->has_dst()) {
				auto frames_dst = get_dst_frames(year);
				frames_month.insert(
						frames_month.end(),
						frames_dst.begin(),
						frames_dst.end());
			}
			return frames_month;
	}
	TimeToEpochConverter(boost::shared_ptr<lt::posix_time_zone> tz, int year_from = 1990, int year_to=2020) : tz(tz){

		for(int i = year_from; i <= year_to; ++i) {
			auto frames = get_frames_per_year(i);
			key_frames.insert(
				key_frames.end(),
				frames.begin(), frames.end()
			);

		}
		std::stable_sort(
				key_frames.begin(), key_frames.end());


	}
	DateTime get_datetime(double epoch){
		auto it = std::lower_bound(key_frames.begin(), key_frames.end(), MonthId{epoch, DateTime{}});
		it--;
		double remainder = epoch - it->epoch;
		DateTime result = it->date;
		result.date.day += (int)(remainder / (3600* 24));
		remainder -= int(remainder / (3600* 24)) * 3600 * 24;

		result.time.hour += (int)(remainder / 3600);

		if(result.time.hour >= 24) {
			// HACK it is not a problem as DST is never on last day of month
			result.date.day++;
			result.time.hour -=24;
		}
		remainder -= int(remainder / 3600) * 3600;
		result.time.min += (int)(remainder / 60);
		remainder -= int(remainder / 60) * 60;
		result.time.sec = remainder;
		return result;
	}

	double get_epoch(DateTime dt) {
		auto it = std::lower_bound(
				key_frames.begin(), key_frames.end(), MonthId{0,dt},
				[](const MonthId & a, const MonthId & b) {
					return a.date < b.date;
				}
		);
		it--;
		double epoch = it->epoch;
		epoch += (dt.date.day - it->date.date.day) * 3600 * 24;
		epoch += (dt.time.hour - it->date.time.hour) * 3600;
		epoch += (dt.time.min - it->date.time.min) * 60;
		epoch += (dt.time.sec - it->date.time.sec);
		return epoch;
	}
};
}
