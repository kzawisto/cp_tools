
#include<gtest/gtest.h>
#include<cp_tools/DateTime.h>
using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace boost::local_time;
using namespace cp_tools;

constexpr auto chicagoTimeSpec = "CST-06CDT+01,M3.2.0/02:00,M11.1.0/02:00";
inline boost::shared_ptr< boost::local_time::posix_time_zone > getTimezone()
{
    static boost::shared_ptr<lt::posix_time_zone> tz= boost::shared_ptr<lt::posix_time_zone> ( new lt::posix_time_zone ( chicagoTimeSpec ) );
    return tz;
}

ptime utc_time_to_chicago_time(ptime utc_time) {
//getTimezone()->dst_local_start_time(greg_year(2012))
	auto local_time_before_dst = utc_time + getTimezone()->base_utc_offset();
	auto dst_start= getTimezone()->dst_local_start_time(local_time_before_dst.date().year());
	auto dst_end= getTimezone()->dst_local_end_time(local_time_before_dst.date().year());
	if(local_time_before_dst < dst_end && local_time_before_dst > dst_start) {
		return local_time_before_dst + getTimezone()->dst_offset();
	}
	else {
		return local_time_before_dst;
	}

}

TEST(time, local_time) {
	/*
	ptime t1(date(2002,Jan,10),
	         time_duration(1,2,3));
	boost::local_time::local_date_time dt(t1, getTimezone());
	std::cout<<getTimezone()->dst_local_end_time(greg_year(2012))<<"\n";
	std::cout<<getTimezone()->dst_local_start_time(greg_year(2012))<<"\n";
	std::cout<<getTimezone()->dst_offset()<<"\n";
	std::cout<<getTimezone()->base_utc_offset()<<"\n";

	std::cout<<dt<<" ";
	std::cout<<dt.is_dst()<<" ";
	std::cout<<dt.utc_time()<<" ";
	std::cout<<dt.time_of_day()<<"\n";

	ptime t2(date(2002,Jul,10),
	         time_duration(1,2,3));
	std::cout<<dt.local_time()<<" "<<utc_time_to_chicago_time(t1)<<"\n";
	std::cout<<local_date_time(t2, getTimezone()).local_time()<<" "<<utc_time_to_chicago_time(t2);
	*/
}
TEST(time, convert_epoch_to_datetime_consistency){
	auto tz = getTimezone();
	TimeToEpochConverter conv(tz, 2000,2010);
	DateTime test_case{Date{2010,12,24}, Time{12,4,10}};
	double epoch = conv.get_epoch(test_case);
	auto dt = conv.get_datetime(epoch);
	ASSERT_EQ(dt, test_case);
}

TEST(time, convert_epoch_to_datetime_correctness){

	auto tz = getTimezone();
	TimeToEpochConverter conv(tz, 2018,2020);

	DateTime test_case{Date{2018,9,4}, Time{19,22,56}};
	ASSERT_EQ(test_case, conv.get_datetime(1536106976));


}
