<h2>pid manager</h2>

<b>To install</b>: make<br>
<b>To make for debugging</b>: make debug<br>
<b>Remove source files</b>: make clean<br>
<b>To Uninstall</b>: make uninstall<br>

it accepts two arguments <em>-s</em>, <em>-t</em>
<em>-s</em> will give maximum sleep for seconds, <em>-t</em> gives number of threads.
Example:<br>
<em>./pid_manager -s=30 -t=6</em>

NOTE:
To check if freed pid is being assigned to another thread, try making large number of threads and make max sleep time low. Eg. <em>./pid_manager -s=3 -t=50</em>
