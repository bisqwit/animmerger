<?php
//TITLE=Animation merger

$title = 'Animation merger';
$progname = 'animmerger';

function usagetext($prog)
{
  exec('/usr/local/bin/'.$prog.' --help', $kk);
  $k='';foreach($kk as $s)$k.="$s\n";
  return $k;
}
 
$text = array(
   '1. Purpose' => "

Animmerger.<br>
Motion detection.<br>
Overlap.

", 'usage:1. Usage' => "

<pre class=smallerpre>".htmlspecialchars(usagetext('animmerger'))."</pre>

", '1. Copying' => "

nesmock has been written by Joel Yliluoma, a.k.a.
<a href=\"http://iki.fi/bisqwit/\">Bisqwit</a>,<br>
and is distributed under the terms of the
<a href=\"http://www.gnu.org/licenses/licenses.html#GPL\">General Public License</a> (GPL).

", '1. Requirements' => "

GNU make and C++ compiler is required to recompile the source code.<br>
libgd is also required.

");
include '/WWW/progdesc.php';
