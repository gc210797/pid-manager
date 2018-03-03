pid manager

it accepts two arguments -s, -t
-s will give maximum sleep for seconds, -t gives number of threads.
Example:
./pid_manager -s=30 -t=6

NOTE:
To check if freed pid is being assigned to another thread, try making large number of threads and make max sleep time low. Eg. ./pid_manager -s=3 -t=50
