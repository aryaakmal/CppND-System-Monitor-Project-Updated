#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long times) { 
   
 int hours, minutes, seconds;
 string hrs, mins, secs;

 hours = times/3600;
 minutes = (times % 3600)/60;
 seconds = ((times % 3600) % 60);

 if( seconds < 10 ) {
  secs = "0"+ std::to_string(seconds);
 }else{
  secs = std::to_string(seconds);
 }
 if( minutes < 10 ) {
  mins = "0"+ std::to_string(minutes);
 }else{
  mins = std::to_string(minutes);
 }
 if( hours < 10 ) {
   hrs = "0" + std::to_string(hours);
 }else{
  hrs = std::to_string(hours);
 }

 return (hrs + ":" + mins + ":" + secs);
}
