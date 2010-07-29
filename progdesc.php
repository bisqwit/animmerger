<?php
//TITLE=Animation merger

$title = 'Animation merger';
$progname = 'animmerger';
$git = 'git://bisqwit.iki.fi/animmerger.git';

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

", 'methods:1. Pixel methods' => "

<table>
<tr><td valign=top>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/original.gif\"
     alt=\"Original animation\">
</td><td valign=top>
As a sample, here is the original animation (712100 bytes).
<p>
The animation was created literally by taking a screenshot
from the NES emulator every frame.<br>
(I hacked the emulator
to automatically produce a screenshot after every frame.)
<p>
What follows below, is a list of the pixel methods supported
by animmerger,<br>
along with example images demonstrating what
that pixel method does.
<p>
The <code>-m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF</code><br>
parameter was given to animmerger to remove the HUD that is 256x16 wide<br>
and begins at 0,8. The hexadecimal numbers listed are these colors:
<table border=1>
 <tr>
  <td style=\"width:32px;height:32px;background:#020202;color:#FFF\">020202</td>
  <td style=\"width:32px;height:32px;background:#A64010;color:#000\">A64010</td>
  <td style=\"width:32px;height:32px;background:#D09030;color:#000\">D09030</td>
  <td style=\"width:32px;height:32px;background:#006E84;color:#FFF\">006E84</td>
  <td style=\"width:32px;height:32px;background:#511800;color:#FFF\">511800</td>
  <td style=\"width:32px;height:32px;background:#FFFFFF;color:#000\">FFFFFF</td>
 </tr>
</table>
This removes the text (white) as well as the blinking coin.

</td></tr></table>
<p>
To be exact, here is the list of commands used to generate these pictures.

<div style=\"margin-left:3em;font-size:10px\">
<code>
# Produce the \"original\" gif from the screenshots (png files produced by the emulator)<br>
(mkdir pano3/q; cd pano3/q<br>
 ln ../*.png . ; mogrify -format gif *.png<br>
 gifsicle -O2 -o ../../demo/original.gif -l0 -d3 *.gif)<br>
<br>
# Produce the static pictures<br>
for s in a l m;do (mkdir \$s;cd \$s ; ../animmerger -p\$s ../pano3/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF;<br> mv tile-0000.png ../demo/method-\$s.png;<br> cd ../demo; pngout-hard method-\$s.png)&amp;done &amp;> /dev/null<br>
<br>
# Produce the changelog animation<br>
for s in c;do (mkdir \$s;cd \$s ; rm tile-????.???; ../animmerger -l30 -p\$s ../pano3/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF;<br> mogrify -format gif tile-*.png ;<br> gifsicle -O2 -o ../demo/method-\$s.gif -l0 -d3 tile-*.gif)&amp;done &amp;>/dev/null<br>
<br>
# Produce the loopinglog animations (vary the loop length)<br>
for l in 4 10 30;do (mkdir o;cd o ; rm tile-????.???; ../animmerger -l\$l -po ../pano3/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF;<br> mogrify -format gif tile-*.png ;<br> gifsicle -O2 -o ../demo/method-ol\"\$l\".gif -l0 -d3 tile-*.gif);done &amp;>/dev/null<br>
<br>
# Produce the changelog animation without HUD removal<br>
for s in c;do (mkdir \$s;cd \$s ; rm tile-????.???; ../animmerger -l30 -p\$s ../pano3/*.png ;<br> mogrify -format gif tile-*.png ;<br> gifsicle -O2 -o ../demo/method-t\$s.gif -l0 -d3 tile-*.gif)&amp;done &amp;>/dev/null
</code></div>

", 'static:1.1. Static methods' => "

", 'average:1.1.1. AVERAGE' => "

The \"average\" method produces a \"motion blur\" effect of the entire
input, reducing it into a single frame.
<p>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-a.png\"
     alt=\"Average\">
<p>
You can see a faint trace of all animated actors that appeared in
the animation. Mario moved very fast so his trace is quite difficult
to spot.

", 'mostused:1.1.1. MOSTUSED' => "

The \"most used\" method produces what might be the background
image for the entire animation.
<p>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-m.png\"
     alt=\"Most used\">
<p>
Note: If there is an actor that sits in a certain location
for a long time, it is also recorded.<br>
In this example, there were none though.<br>
This mode does not thus remove all actors, but it does remove
anything that wanders around.

", 'last:1.1.1. LAST'=> "

The \"last\" method is a simpler implementation of the \"most used\"
method, simply recording the last pixel value in any location.
<p>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-l.png\"
     alt=\"Last\">

", 'animated:1.1. Animated methods' => "

", 'changelog:1.1.1. CHANGELOG' => "

The \"changelog\" method records the entire animation (121995 bytes in this example).<br>
It takes considerably less disk space (and network bandwidth) than the original animation, because now the background does not scroll.
<p>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-c.gif\"
     alt=\"Changelog\">
<p>
You see some artifacts in the turtle and in Mario when they appear
near the top of the screen. This is because they were behind the
HUD (the text \"WORLD 8-2\" for instance), which was removed.<br>
In the case of the turtle, the turtle's white pixels were also
removed, because the HUD removal was based on color as well
as coordinates.<br>
Horizontal disappearance of the actors is because of the viewport
scrolling past them. They do not exist outside those parameters
in the original animation either.
<p>
Here is how the animation looks like, if the HUD is not removed. (246643 bytes)
<p>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-tc.gif\"
     alt=\"Changelog, with HUD intact\">
<p>
Exteriors, i.e. content outside the \"current\" viewport of the animation
are colored as in the \"most used\" pixel method.<br>
This is evident in the trails left by the HUD as it scrolls by at different speeds.

", 'loopinglog:1.1.1. LOOPINGLOG' => "

The \"loopinglog\" method records the entire animation,
but reuses existing frames. Use the <code>-l</code> option
to set the loop length in frames.<br>
The smaller value you use, the shorter the animation
is in the number of frames, but the more pronounced
is the \"lemmings\" effect.
<p>
30 frames (99755 bytes):<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-ol30.gif\"
     alt=\"Loop, 30 frames\">
<p>
10 frames (73850 bytes):<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-ol10.gif\"
     alt=\"Loop, 10 frames\">
<p>
4 frames (49890 bytes):<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-ol4.gif\"
     alt=\"Loop, 4 frames\">

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
