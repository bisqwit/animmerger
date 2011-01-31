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

Animmerger stitches 2D images together into either a static image
or an animation, while attempting to preserve a global frame of
reference (static background).<br> 
That is, for a movie that follows
an actor around (and the background scrolls to follow them), it
creates a movie that has a fixed background, and the camera moves
around in the scene.
<p>
It does this with a motion detection algorithm, a set of
different pixel methods, and a simulated infinite 2D canvas
&mdash; a 2D canvas that extends infinitely to all four directions
(well, as infinite as 32-bit integers can get&hellip;)

", 'methods:1. Pixel methods' => "

<table>
<tr><td valign=top>
<img width=256 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/original.gif\"
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
</td></tr></table><br>
<small>
The graphics material comes from Super Mario Bros.<br>
Mario, Super Mario Bros., and The Nintendo Entertainment System (NES)
are registered trademarks of Nintendo of America Inc.
But you knew that, right?</small>

", 'static:1.1. Static methods' => "

", 'average:1.1.1. AVERAGE' => "

The \"average\" method produces a \"motion blur\" effect of the entire
input, reducing it into a single frame.
<p>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-a.png\"
     alt=\"Average\">
<p>
You can see a faint trace of all animated actors that appeared in
the animation. Mario moved very fast so his trace is quite difficult
to spot.
<p>
Produced with commandline:<br>
<code># animmerger -pa snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# mv tile-0000.png demo/method-a.png</code>
<p>
An alternative implementation of \"average\" is also provided: \"tinyaverage\" (option -A).
It requires less memory to store, but is less accurate to calculate.
<p>
If you want the color averages to be calculated through the YUV colorspace
rather than the RGB colorspace, add the --yuv option (not supported by tinyaverage).

", 'actionavg:1.1.1. ACTIONAVG' => "

The \"actionavg\" method attempts to fix the faintness problem with
\"average\" method by keeping track separately of the background
(using the \"mostused\" method) and adding it only once to the
average of moving actors.
<p>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-t.png\"
     alt=\"Action average\">
<p>
Produced with commandline:<br>
<code># animmerger -pt snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# mv tile-0000.png demo/method-t.png</code>
<p>
If you want the color averages to be calculated through the YUV colorspace
rather than the RGB colorspace, add the --yuv option.

", 'mostused:1.1.1. MOSTUSED' => "

The \"most used\" method produces what might be the background
image for the entire animation.
<p>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-m.png\"
     alt=\"Most used\">
<p>
Note: If there is an actor that sits in a certain location
for a long time, it is also recorded.<br>
In this example, there were none though.<br>
This mode does not thus remove all actors, but it does remove
anything that wanders around.
<p>
Produced with commandline:<br>
<code># animmerger -pm snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# mv tile-0000.png demo/method-m.png</code>

", 'last:1.1.1. LAST'=> "

The \"last\" method is a simpler implementation of the MostUsed
method, simply recording the last pixel value in any location.
<p>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-l.png\"
     alt=\"Last\">
<p>
Produced with commandline:<br>
<code># animmerger -pl snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# mv tile-0000.png demo/method-l.png</code>

", 'last:1.1.1. FIRST'=> "

The \"first\" method is analogous to \"last\".
It shows whatever first appeared in a particular pixel location.
<p>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-f.png\"
     alt=\"First\">
<p>
The turtles are distorted, because they moved while the screen scrolled.<br>
It is the same effect as if you move the paper in a desktop scanner during the scanning.
<p>
Produced with commandline:<br>
<code># animmerger -pf snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# mv tile-0000.png demo/method-f.png</code>

", 'solid:1.1.1. SOLID'=> "

The \"solid\" method is an experimental light-weight replacement
to the \"mostused\" method. It simply ignores anything that moves
and retains whatever stays still for the longest time.<br>
Unlike \"mostused\",
it does not sum separate appearances together; it only finds the maximum
length of consecutive sameness.
<p>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-O.png\"
     alt=\"Solid\">
<p>
As seen here, it has shortcomings, too.
<p>
Produced with commandline:<br>
<code># animmerger -pO snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# mv tile-0000.png demo/method-O.png</code>

", 'firstnmost:1.1.1. FIRSTNMOST'=> "

The \"firstnmost\" method is analogous to \"first\" and \"mostused\";
it chooses the most common pixel of first N pixel values.
Set N with the --firstlast (-f) option.<br>
If N is 0, instead gets last uncommon pixel.<br>
If N is negative, using least common values rather than most common.
<p>
Most common of first 4:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Ff4.png\"
     alt=\"Most common of first 4\">
<p>
Most common of first 10:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Ff10.png\"
     alt=\"Most common of first 10\">
<p>
Most common of first 16:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Ff16.png\"
     alt=\"Most common of first 16\">
<p>
First uncommon:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Ff0.png\"
     alt=\"First uncommon\">
<p>
Least common of first 10:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Ff-10.png\"
     alt=\"Least common of first 10\">
<p>
Produced with commandline:<br>
<code># for f in 4 10 -10 16 0; do <br>
# &nbsp; animmerger -pF -f\$f snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# &nbsp; mv tile-0000.png demo/method-Ff\$f.png<br>
# done</code>

", 'lastnmost:1.1.1. LASTNMOST'=> "

The \"lastnmost\" method is analogous to \"last\" and \"mostused\";
it chooses the most common pixel of last N pixel values.
Set N with the --firstlast (-f) option.<br>
If N is 0, instead gets last uncommon pixel.<br>
If N is negative, using least common values rather than most common.
<p>
Most common of last 10:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Lf10.png\"
     alt=\"Most common of last 10\">
<p>
Last uncommon:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Lf0.png\"
     alt=\"Last uncommon\">
<p>
Least common of last 10:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Lf-10.png\"
     alt=\"Least common of last 10\">
<p>
Produced with commandline:<br>
<code># for f in 4 10 -10 16 0; do <br>
# &nbsp; animmerger -pL -f\$f snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# &nbsp; mv tile-0000.png demo/method-Lf\$f.png<br>
# done</code>

", 'leastused:1.1.1. LEASTUSED' => "

The \"least used\" method is analogous to \"most used\".<br>
It can be used to find graphical artifacts and teleporting actors,
but for the most part, the output is not very useful.
<p>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-e.png\"
     alt=\"Least used\">
<p>
Produced with commandline:<br>
<code># animmerger -pe snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# mv tile-0000.png demo/method-e.png</code>

", 'animated:1.1. Animated methods' => "

", 'changelog:1.1.1. CHANGELOG' => "

The \"changelog\" method records the entire animation (121995 bytes in this example).<br>
It takes considerably less disk space (and is faster to load) than the original animation, because now the background does not scroll.
<p>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-c.gif\"
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
<img width=725 height=225
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-tc.gif\"
     alt=\"Changelog, with HUD intact\">
<p>
Exteriors, i.e. content outside the \"current\" viewport of the animation
are colored as in the MostUsed pixel method.<br>
This is evident in the trails left by the HUD as it scrolls by at different speeds.
<p>
Produced with commandline:<br>
<code>
# rm tile-*.png tile-*.gif<br>
# animmerger --gif -pc snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# <a href=\"http://www.lcdf.org/gifsicle/\">gifsicle</a> -O2 -o demo/method-c.gif -l0 -d3 tile-*.gif</code><br>
The version with HUD intact was created with the same commandline,
except with the -m option removed.
 <p>
The background for ChangeLog is normally generated with the MostUsed method, but it can be
explicitly controlled with the <tt>--bgmethod0</tt> and <tt>--bgmethod1</tt> options.<br>
Here is how the above animation (HUD-less) looks like with <tt>--bgmethod0 first --bgmethod1 last</tt>:
 <p>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-cfl.gif\"
     alt=\"Changelog with first&amp;last\">
<br>
Note that the <tt>--bgmethod0</tt> and <tt>--bgmethod1</tt> options
only affect the ChangeLog method, and only when motion blur is not used.

", 'motionblur:1.1.1.1. Motion blur' => "

The changelog method also supports motion blur. Use the --motionblur (-B) option to set it.
Value 0 disables motion blur (default: 0).
<p>
Blur length 1:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-cB1.gif\"
     alt=\"Changelog, blur 1\">
<p>
Blur length 4:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-cB4.gif\"
     alt=\"Changelog, blur 4\">
<p>
Blur length 20:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-cB20.gif\"
     alt=\"Changelog, blur 1\">
<p>
Produced with commandline:<br>
<code>
# for b in 1 4 20;do <br>
# &nbsp; rm tile-*.png tile-*.gif<br>
# &nbsp; animmerger --gif -B\$b -pc snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# &nbsp; gifsicle -O2 -o demo/method-cB\"\$b\".gif -l0 -d3 tile-*.gif<br>
# done</code>

", 'loopinglog:1.1.1. LOOPINGLOG' => "

The \"loopinglog\" method records the entire animation,
but reuses existing frames. Use the <code>-l</code> option
to set the loop length in frames.<br>
The smaller value you use, the shorter the animation
is in the number of frames, but the more pronounced
is the \"lemmings\" effect.
<p>
30 frames (94895 bytes):<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-sl30.gif\"
     alt=\"Loop, 30 frames\">
<p>
10 frames (66738 bytes):<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-sl10.gif\"
     alt=\"Loop, 10 frames\">
<p>
4 frames (40372 bytes):<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-sl4.gif\"
     alt=\"Loop, 4 frames\">
<p>
Produced with commandline:<br>
<code>
# for l in 4 10 30; do <br>
# &nbsp; rm tile-*.png tile-*.gif<br>
# &nbsp; animmerger --gif -l\$l -po snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# &nbsp; gifsicle -O2 -o demo/method-sl\"\$l\".gif -l0 -d3 tile-*.gif<br>
# done</code>
 <p>
It is also called \"loopinglast\" mode (option -s)
to differentiate from \"loopingavg\".
 <p>
The loopinglog method also supports motion blur.
Use the --motionblur (-B) option to set it.
Value 0 disables motion blur (default: 0).

", 'loopingavg:1.1.1. LOOPINGAVG' => "

The \"loopingavg\" method combines the \"loopinglog\" and \"actionavg\" methods.
Use the <code>-l</code> option to set the loop length in frames.<br>
The most important difference to \"loopinglog\" is that overlapping action
is averaged rather than explicitly choosing one of the acting pixels.<br>
It looks slightly better, but may require GIF palette reduction.<br>
In comparison, \"loopinglog\" only uses pixel colors present
in the original images.
<p>
30 frames (file size depends on selected palette size):<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl30.gif\"
     alt=\"Loop, 30 frames\">
<p>
10 frames:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl10.gif\"
     alt=\"Loop, 10 frames\">
<p>
4 frames:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl4.gif\"
     alt=\"Loop, 4 frames\">
<p>
Produced with commandline:<br>
<code>
# for l in 4 10 30 80; do <br>
# &nbsp; rm tile-*.png tile-*.gif<br>
# &nbsp; animmerger --gif -l\$l -pv snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# &nbsp; gifsicle -O2 -k128 -o demo/method-ov\"\$l\".gif -l0 -d3 tile-*.gif<br>
# done</code>
<p>
If you want the color averages to be calculated through the YUV colorspace
rather than the RGB colorspace, add the --yuv option.<br>
In most cases, the difference is neglible though.

", 'avgblur:1.1.1.1. Motion blur' => "

The loopingavg method also supports motion blur.
Use the --motionblur (-B) option to set it.
Value 0 disables motion blur (default: 0).
<p>
Loop length 30 frames, blur length 20:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl30B20.gif\"
     alt=\"Loop-Avg 30, blur 8\">
<p>
Loop length 30 frames, blur length 20, with YUV calculations:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl30yB20.gif\"
     alt=\"Loop-Avg 30 through YUV, blur 8\">
<p>
Loop length 30 frames, blur length 20, with YUV calculations, and diversity-quantized palette of 16 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl30yB20Qd16.gif\"
     alt=\"Loop-Avg 30 through YUV, blur 8, 16 colors\">
<p>
Loop length 10 frames, blur length 4:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl10B4.gif\"
     alt=\"Loop-Avg 10, blur 4\">
<p>
Produced with commandline:<br>
<code>
# for b in 4 8 20;do <br>
# &nbsp; for l in 10 30;do <br>
# &nbsp; &nbsp; rm tile-*.png tile-*.gif<br>
# &nbsp; &nbsp; animmerger --gif -B\$b -l\$l -pl snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# &nbsp; &nbsp; gifsicle -O2 -o demo/method-vl\"\$l\"B\"\$b\".gif -l0 -d3 tile-*.gif<br>
# &nbsp; done<br>
# done</code>

", 'methodsummary:1.1. Summary' =>"

<table border=1 style=\"text-align:left\">
 <tr valign=bottom>
  <th>Method name</th>
  <th>Static or<br>animated</th>
  <th>Composes<br>new colors</th>
  <th>Obeys YUV<br>option</th>
  <th>Memory size per pixel*</th>
  <th>Primary<br>use</th>
 </tr><tr><th>First</th>                  <td>Static          </td>   <td>No</td>     <td>No       </td><td>4</td>                                          <td>Maps</td>
 </tr><tr><th>Last</th>                   <td>Static          </td>   <td>No</td>     <td>No       </td><td>4</td>                                          <td></td>
 </tr><tr><th>FirstNMost</th>             <td>Static          </td>   <td>No</td>     <td>No       </td><td>As ChangeLog</td>                               <td></td>
 </tr><tr><th>&middot; FirstUncommon</th> <td>Static          </td>   <td>No</td>     <td>No       </td><td>As ChangeLog</td>                               <td></td>
 </tr><tr><th>&middot; FirstNLeast</th>   <td>Static          </td>   <td>No</td>     <td>No       </td><td>As ChangeLog</td>                               <td></td>
 </tr><tr><th>LastNMost</th>              <td>Static          </td>   <td>No</td>     <td>No       </td><td>As ChangeLog</td>                               <td></td>
 </tr><tr><th>&middot; LastUncommon</th>  <td>Static          </td>   <td>No</td>     <td>No       </td><td>As ChangeLog</td>                               <td></td>
 </tr><tr><th>&middot; LastNLeast</th>    <td>Static          </td>   <td>No</td>     <td>No       </td><td>As ChangeLog</td>                               <td></td>
 </tr><tr><th>MostUsed</th>               <td>Static          </td>   <td>No</td>     <td>No       </td><td>12&hellip;16 + 6&times;number of unique colors</td>   <td>Maps</td>
 </tr><tr><th>LeastUsed</th>              <td>Static          </td>   <td>No</td>     <td>No       </td><td>As MostUsed</td>                                <td></td>
 </tr><tr><th>Solid</th>                  <td>Static          </td>   <td>No</td>     <td>No       </td><td>12</td>                                         <td>Maps</td>
 </tr><tr><th>Average</th>                <td>Static          </td>   <td>Yes</td>    <td>Yes      </td><td>16</td>                                         <td></td>
 </tr><tr><th>Tinyaverage</th>            <td>Static          </td>   <td>Yes</td>    <td>No       </td><td>8</td>                                          <td></td>
 </tr><tr><th>ActionAvg</th>              <td>Static          </td>   <td>Yes</td>    <td>Yes      </td><td>As MostUsed</td>                                <td></td>
 </tr><tr><th>ChangeLog</th>              <td>Animated (movie)</td> <td>If blur</td><td>For blur </td><td>12&hellip;16 + 8&times;number of content changes</td> <td>Animations</td>
 </tr><tr><th>LoopingLog</th>             <td>Animated (loop) </td> <td>If blur</td><td>For blur </td><td>As ChangeLog</td>                               <td></td>
 </tr><tr><th>LoopingAvg</th>             <td>Animated (loop) </td> <td>Yes</td>    <td>Yes      </td><td>As ChangeLog</td>                               <td>Fun</td>
 </tr>
</table>
 <p>
*) These numbers are estimates. Actual memory size per pixel depends
on the exact selection of pixel methods requested and the memory
allocation overhead. Animmerger strives
to always select the smallest combination of pixel methods
(memoryconsumptionwise) that can implement all the requested methods.

", 'maskmethods:1. Masking methods' => "

Masked areas can be removed with a number of different methods.
To best demonstrate them, I added an extra huge mask in the middle of the image.<br>
It is best seen in the \"black\" masking, below.
<p>
These images were produced with this commandline:<br>
<code>
# for method in censor hole interpolate extrapolate; do<br>
# &nbsp; rm *-*.gif *-*.png<br>
# &nbsp; ./animmerger -r4,4 --mvrange 0,0,4,0 --bgmethod0=first --bgmethod1=last \\<br>
# &nbsp;     -u\$method -p* snaps/*.png \\<br>
# &nbsp;     -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF \\<br>
# &nbsp;     -m3,128,250,72  -m0,73,256,2<br>
# &nbsp; gifsicle -O2 -o demo/mask-\$method.gif -l0 -d3 ChangeLog-*.gif<br>
# &nbsp; cp -p Average-0000.png demo/mask-\$method.png<br>
# done</code>

", 'maskmethod_black:1.1. BLACK/BLANK/CENSOR' => "

This method shows clearly which areas were affected by the mask.
Specifically, the HUD, and a huge rectangle,
and a narrower line extending from the very left edge to the very right
edge of the screen at all times, effectively blocking the contents of
the entire scanline from ever being seen.
<p>
Animation:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/mask-censor.gif\"
     alt=\"Masked with CENSOR, animation\"><br>
Averaged:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/mask-censor.png\"
     alt=\"Masked with CENSOR, average\">

", 'maskmethod_hole:1.1. HOLE/ALPHA/TRANSPARENT' => "

This method is what animmerger does by default. The transparent regions
are simply treated as holes; there is no content on the affected areas.
If the hidden content becomes available when the camera moves, then those
pixels are recorded.
<p>
Animation:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/mask-hole.gif\"
     alt=\"Masked with HOLE, animation\"><br>
Averaged:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/mask-hole.png\"
     alt=\"Masked with HOLE, average\">

", 'maskmethod_interpolate:1.1. DELOGO/BLUR/INTERPOLATE' => "

This method removes the content with a circular blur pattern. The method
is almost identical to the <i>delogo</i> filter that can be used in
<a href=\"http://mplayerhq.hu/\">MPlayer</a>
to remove a tv station logo from video. Content that coindices with the
removed part is replaced with interpolated surrounding pixels;
original pixels of the affected area are not sampled.
<p>
Animation (palette-reduced and dithered with -Qd,16 in order to make the 1.5 MB GIF file smaller):<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/mask-interpolate.gif\"
     alt=\"Masked with DELOGO, animation\">
</a><br>
Averaged:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/mask-interpolate.png\"
     alt=\"Masked with DELOGO, average\">

", 'maskmethod_extrapolate:1.1. PATTERN/EXTRAPOLATE' => "

The extrapolate filter tries to extrapolate the content of the masked
areas by detecting repeating tile patterns outside the masked area, and
extrapolating those patterns over the masked area.
The results of this method vary a lot from frame to frame,
so it is not very suitable to be used over large unknown areas.
For small areas, it works nicely.
<!--Of these four, this is the only filter for which the
    thin entire-scanline mask did not pose any kind of problem.-->
 <p>
Note that this algorithm is rather slow on large areas like this.
<p>
Animation:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/mask-extrapolate.gif\"
     alt=\"Masked with PATTERN, animation\"><br>
Averaged:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/mask-extrapolate.png\"
     alt=\"Masked with PATTERN, average\">

", 'palettemethods:1. Color quantization methods' => "

Animmerger can create its output files in GIF or PNG format,
regardless of whether you are creating an animation or not.<br>
GIF files however are limited to a palette of 256 colors,
while it is possible that animmerger creates images with more
colors than 256. Therefore, the colormap must be reduced
before the GIF image can be generated. animmerger supports
a number of different color reduction methods, which are
listed below.<br>
If no method is chosen, whatever is libGD default will be used.
 <p>
The images in this section were generated by making a 30-frame <a href=\"#loopingavg\">LoopingAvg</a>
animation with <a href=\"#avgblur\">blur length</a> of 20, rendering it with different palettization
parameters and picking the 11th frame.
 <p>
The exact commandline to produce the images was:<br>
<code># for m in m d b o q; do<br>
# &nbsp;for q in 2 4 8 16 32 64 128; do<br>
# &nbsp; rm tile-*.png tile-*.gif<br>
# &nbsp; ./animmerger --gif --yuv -Q\"\$m\",\$q -B20 -l30 -pv pano3/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# &nbsp; gifsicle -O2 -k128 -o demo/method-vl30yB20Q\"\$m\"\$q.gif -l0 -d3 tile-*.gif<br>
# &nbsp; convert tile-0010.gif -quality 100 demo/quant-\"\$c\"\$q.png<br>
# &nbsp;done<br>
# done</code>
 <p>
Palette reduction methods can be chained in order to take benefits
of the differently-appearing strengths of the different methods,
but in this test set, each method was used alone.
 <p>
When palette reduction methods have been explicitly selected, animmerger
always uses an ordered-dithering method (crosshatch artifacts) to optimize
the rendering. This is better for animation than other methods such
as Floyd-Steinberg are, because the dithering patterns do not jitter
between frames.

", 'palette_heckbert:1.1. Median-cut (aka. Heckbert)' => "

Heckbert median-cut quantization method repeatedly splits the palette
into two roughly equal-proportion sections (\"low\" and \"high\" part
in any of red/green/blue channels) until the desired number of sections
have been generated; the palette is generated by averaging the values
in each section together.<br>
It is good at generating relevant palettes, but at smallest palette
sizes, it suffers from a blurring problem.
<p>

4 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-m4.png\"
     alt=\"Heckbert quantization, 4 colors\"><br>
8 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-m8.png\"
     alt=\"Heckbert quantization, 8 colors\"><br>
16 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-m16.png\"
     alt=\"Heckbert quantization, 8 colors\"><br>
32 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-m32.png\"
     alt=\"Heckbert quantization, 32 colors\">

", 'palette_diversity:1.1. Diversity' => "

Diversity quantization method alternates between choosing the most popular
remaining color in the image for a \"seed\" and choosing of the remaining
colors the one that is most distant to any colors selected so far.<br>
The result is generally a very good representation of the original
image's colors.<br> At the smallest palette sizes, the colors are
of course off, but the contrast is still sharp.
<p>

4 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-d4.png\"
     alt=\"Diversity quantization, 4 colors\"><br>
8 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-d8.png\"
     alt=\"Diversity quantization, 8 colors\"><br>
16 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-d16.png\"
     alt=\"Diversity quantization, 16 colors\"><br>
32 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-d32.png\"
     alt=\"Diversity quantization, 32 colors\">

", 'palette_blenddiversity:1.1. Blend-diversity' => "

The blend-diversity method is a variation to the diversity method; after
the colors have been chosen, they are merged together with the remaining
colors that are most similar to the chosen one.
<p>

4 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-b4.png\"
     alt=\"Blend-diversity quantization, 4 colors\"><br>
8 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-b8.png\"
     alt=\"Blend-diversity quantization, 8 colors\"><br>
16 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-b16.png\"
     alt=\"Blend-diversity quantization, 16 colors\"><br>
32 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-b32.png\"
     alt=\"Blend-diversity quantization, 32 colors\">

",/* 'palette_octree:1.1. Octree' => "

4 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-o4.png\"
     alt=\"Octree quantization, 4 colors\"><br>
8 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-o8.png\"
     alt=\"Octree quantization, 8 colors\"><br>
16 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-o16.png\"
      alt=\"Octree quantization, 16 colors\"><br>
32 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-o32.png\"
     alt=\"Octree quantization, 32 colors\">

",*/ 'palette_neuquant:1.1. NeuQuant' => "

The NeuQuant method, developed by Anthony Dekker in 1994, uses a Kohonen
self-balancing neural network to quickly come up with an optimized palette.
It is especially powerful with optimizing smooth gradients, such as the
motion-blur trails in this pictureset.
<p>

4 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-q4.png\"
     alt=\"NeuQuant quantization, 4 colors\"><br>
8 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-q8.png\"
     alt=\"NeuQuant quantization, 8 colors\"><br>
16 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-q16.png\"
     alt=\"NeuQuant quantization, 16 colors\"><br>
32 colors:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/quant-q32.png\"
     alt=\"NeuQuant quantization, 32 colors\">

", 'dither:1. Dithering' => "

Dithering is a technique by which the human eye can be tricked into
perceiving more colors than there actually is, by putting different-colored
pixels adjacent next to each others in varying proportions.
 <p>
Animmerger uses a type of
<a href=\"http://bisqwit.iki.fi/story/howto/dither/jy/\">Knoll-Yliluoma</a>
dithering; an algorithm that resembles the dithering employed
by Adobe® Photoshop® , but is based on a different set of ideas
and is more versatile and configurable.
It is categorised as an ordered, positional, patterned dithering method
that is very well suited for animations, and often even more eye-pleasing
than the random noise patterns generated by error diffusion dithers.
 <p>
Animmerger's dithering can be controlled with the following parameters:

<dl>
 <dt style=\"font-family:monospace\"> --ditherror, --de &lt;float&gt; </dt>
   <dd> <b>Color error spectrum factor</b> is a parameter
     that controls how strongly the ditherer will attempt
     to find the next color value to mix with the previous
     ones. A value of 0 will effectively disable dithering,
     and a value of 1 is the algorithm working at full power.
     Values in the between have effect that depends on the particular palette. </dd>
  
 <dt style=\"font-family:monospace\"> --dithmatrix, --dm &lt;x&gt;,&lt;y&gt;[,&lt;time&gt;] </dt>
  <dd> <b>Bayer matrix dimensions</b> define very straightforwardly
     the visual appearance of the dithering. A small matrix appears
     very regular and a larger matrix looks more random. The matrix
     dimensions should generally be powers of two in both directions,
     though this is not required.<br>
     An additional third dimension can be specified: This dimension is
     time. If dithering along the time axis (temporal dithering) is
     requested, then flickering will be used to achieve the desired
     average colortone over time rather than over spatial distance.
     The value is the number of frames over which to extend the dithering.
     A negative value can be given; in this case, the dithering will be
     performed from the higher-order bits (most prominent) rather than
     the lowest-order bits (least prominent).</dd>
  
 <dt style=\"font-family:monospace\"> --dithcount, --dc &lt;int&gt; </dt>
  <dd> Sets the <b>maximum number of palette colors</b> to mix when
     attempting to reproduce a color. Value should be at least 1
     (1 disables dithering) and at most the size of the dithering matrix.</dd>
  
 <dt style=\"font-family:monospace\"> --dithcontrast, --dr &lt;float&gt; </dt>
  <dd> The contrast parameter is used to control how palette colors are
     pre-mixed as candidates for the in-render color chooser to use.
     A larger value will produce more pre-mixed candidates
     (with a coarser appearance), a smaller value will inhibit
     these candidates.</dd>
  
</dl>

", 'dither_sample:1.1. Example' => "

To demonstrate dithering, let us consider this example picture.
It has been assigned a customized palette to go with it.
 <p>
<img width=80 height=140
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/dith_base.png\"
     alt=\"Base picture for dithering\">
<img width=168 height=72
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/dith_pal.png\"
     alt=\"Palette to go with the dithering demo\">
 <p>
It is a subset (cropped portion) of a larger picture seen on
the page where the algorithm is
<a href=\"http://bisqwit.iki.fi/story/howto/dither/jy/\">explained in detail</a>,
hence the odd inclusion of blue in it.

", 'dither_error:1.1.1. Dither error spread factor' => "

<img width=656 height=159
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/dith_de.png\"
     alt=\"Adjusting --ditherror option.\">
 <p>
The error spread factor provides a very fine-grained control over
the final appearance of the dithered image. Though the upper limit
of the value is 1.0, higher values can be used for artistic purposes.

", 'dither_matrix:1.1.1. Dither matrix size' => "

<img width=656 height=159
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/dith_dm.gif\"
     alt=\"Adjusting --dithmatrix option.\">
 <p>
The matrix shape directly controls the manner in which the different-color
spots are dispersed. The temporal dithering option can be used for
improving the perceived quality of colors (at the cost of flickering),
and for artistics effects.
 <p>
Unless the --dithcount (--dc) option was given manually, setting the
matrix size also sets the former.
(To the size of matrix, or 32, whichever is smaller.)
 <p>
Note that when making GIF animations, you usually do not want flickering,
because it will inflate the file sizes at a very high rate. With H.264,
it is perfectly fine, especially if you use the <code>frameref</code>
setting. (No, Animmerger does not have H.264 output. I was just referring
to the <a href=\"http://www.youtube.com/watch?v=-Nz_QB26Clw\"
>hypothetical scenario</a> that you would use animmerger to create
<a href=\"http://www.youtube.com/watch?v=9YoqUM1ZLQ0\"
>a video</a> that will be encoded in H.264.)

", 'dither_count:1.1.1. Dither candidate count' => "

<img width=574 height=178
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/dith_dc.png\"
     alt=\"Adjusting --dithcount option.\">
 <p>
The candidate count option directly controls how colors are mixed
together in the dithering process. A higher value always results
in higher quality, however, there is no sense in making the value
larger than the matrix size is. Also, a combination of a large
matrix and a small count can be used to simulate a small dithering matrix.
 <p>
Also note that the rendering speed is directly proportional to the number
of dither color candidates generated. (It also depends on the size of the
palette of both input and output images, and on the dither contrast limiter.)

", 'dither_contrast:1.1.1. Dither contrast limiter' => "

<img width=656 height=197
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/dith_dr.gif\"
     alt=\"Adjusting --dithcontrast option.\">
 <p>
Specifying 0 for the contrast usually works nicely, especially if the
palette is good, but sometimes you will have to put a higher value there.
Such situations may happen if the palette contains a combination of colors
that produces the exact color required in the input picture when mixed, but
also a closeby color that is not exact. Without the aid of the contrast option,
the ditherer will not find the combination and will just use the closerby color
that might not look as good.
Overdoing it, however, will result in a lot of overly sharp local contrast,
which looks mostly bad. Animation is shown in the last frame for the sake
of demonstration, because it improves the spatial color resolution.

", 'dither_compare:1. Color compare methods' =>  "

In dithering, a color compare algorithm is used. The same algorithm
is also used in the diversity and blend-diversity quantization options.
Animmerger supports a few different algorithms for comparing colors.
<p>

Here are two example truecolor* pictures, and the
<a href=\"http://en.wikipedia.org/wiki/Web_colors\">web-safe palette</a>.<br>
<img width=256 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/deltae_base.png\"
     alt=\"Color scales at various luminosity and saturation.\"
     title=\"Luma-weighted hue/value scales showed at various saturation levels.\">
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tksmall.png\"
     alt=\"A faux testcard for God's Learning Channel\"
     title=\"A faux testcard that Joel created in honor of God's Learning Channel.\">
<img width=248 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/webpal.png\"
     alt=\"Palette to go with the color compare demo.\"
     title=\"Web-safe palette.\">
<p>
I quantized it using the websafe palette and dithered using
<code>--dm 1x1 --dc 1 --dr 0</code> (i.e. no dithering),
and varied the color compare method using the <code>--cie</code> option.
<p>
These tests intend to show how each color-compare method
identifies colors that most closely match the original.
<p>
Produced with commandline:<br>
<code># for e in rgb cie76 cie94 cmc ciede2000 bfd; do <br>
# &nbsp; # Render the chroma&amp;luma test image without dithering:<br>
# &nbsp; animmerger deltae_base.png -Qdeltae_pal.png -vv \\<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
        --dm=1x1 --dc=1 --dr=0 --cie=\$e --gamma 2.0<br>
# &nbsp; mv tile-0000.png demo/deltae_\$e.png<br>
#<br>
# &nbsp; # Create four-frame temporal-dithered animation of the testcard:<br>
# &nbsp; animmerger tksmall{,,,}.png --noalign -Qdeltae_pal.png -vv \\<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
        --dm 1x1,-4 --dc 4 --dr 1.4 --cie=\$e -pc --gif --gamma 2.0<br>
# &nbsp; gifsicle -O2 -o demo/tk-\$e.gif -l0 -d4 tile-000[0-3].gif<br>
#<br>
# &nbsp; # Create an average of those four frames:<br>
# &nbsp; animmerger tile-000[0-3].gif -pa --noalign<br>
# &nbsp; mv tile-0000.png tk-a4-\$e.png<br>
# done</code>
 <p>
*) It is truecolor, but it is also dithered.
I found 24-bit RGB inadequate for this picture in preventing
hard edges in smooth gradients, so I dithered it for this webpage.
The input to these tests was undithered.

", 'cie_rgb:1.1. RGB' => "

Three pictures are shown:
The two testcases rendered with this color filter, and the third
is a <i>four-frame average</i> of the preceding picture, showing
exactly which average color perception the dithering was getting at.
<p>

<img width=256 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/deltae_rgb.png\"
     alt=\"Colors compared in RGB\" align=left>
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-rgb.gif\"
     alt=\"Colors compared in RGB\" align=left>
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-a4-rgb.png\"
     alt=\"4-frame average\" align=left>
<br>
RGB. Calculated as a simple euclidean difference:
√(ΔR² + ΔG² + ΔB²)
<br>
<br>
It is very fast and does not usually cause any nasty surprises.
However, in dithering, it usually is overly conservative and fails
to account for psychovisuals, i.e. when some color is \"close enough\",
and consequently, fails to achieve a pleasing result. For instance, the
magenta square is much more wildly animated in this version than in any
other, and its average color is also farthest from the original.
Same goes for the lightgrey squares right above it,
and likely many other details.
<br clear=all>

", 'cie_cie76:1.1. CIE76' => "

<img width=256 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/deltae_cie76.png\"
     alt=\"Colors compared in CIE76\" align=left> 
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-cie76.gif\"
     alt=\"Colors compared in CIE76\" align=left>
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-a4-cie76.png\"
     alt=\"4-frame average\" align=left>
<br>
CIE L*a*b*, where delta-E calculated as a simple euclidean difference:
√(ΔL² + Δa² + Δb²)
<br><br>
It is fast and very often an improvement to RGB, and it is indeed visually
difficult to spot anywhere that this algorithm did wrong. That is what
psychovisuality is about, knowing what humans can distinguish and what
they cannot. For example, the upper-right tall olive rectangle is
considerably darker here than it was in the original, but it is not
easy to notice it, is it?

<br clear=all>

", 'cie_cie94:1.1. CIE94' => "

<img width=256 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/deltae_cie94.png\"
     alt=\"Colors compared in CIE94\" align=left> 
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-cie94.gif\"
     alt=\"Colors compared in CIE94\" align=left>
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-a4-cie94.png\"
     alt=\"4-frame average\" align=left>
<br>
CIE L*a*b* with C<sub>ab</sub>=√(a²+b²) and h<sub>ab</sub>=tan<sup>-1</sup>(b÷a),<br>
where delta-E calculated using a much more refined formula (CIE94):<br>
√(ΔL² + ΔC<sub>ab</sub>²÷S<sub>C</sub>² + ΔH÷S<sub>h</sub>²)<br>
with ΔH = (Δa² + Δb² − ΔC<sub>ab</sub><sup>2</sup>),<br>
and S<sub>C</sub> = (1 + 0.048×√(C<sub>1ab</sub>×C<sub>2ab</sub>)),<br>
and S<sub>h</sub> = (1 + 0.014×√(C<sub>1ab</sub>×C<sub>2ab</sub>)).<p>
Note: Animmerger uses the deltaE squared rather than the deltaE itself,
which is why the formula may seem different to what it is in reference
material. (There may still be genuine errors though.)
<br><br>
One symptom of such error might be in the aforementioned supposedly olive
rectangle at the top right area of the screen. It is rendered at a
suspiciously bright tone here.

<br clear=all>

", 'cie_ciede2000:1.1. CIEDE2000' => "

<img width=256 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/deltae_ciede2000.png\"
     alt=\"Colors compared in CIEDE2000\" align=left>
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-ciede2000.gif\"
     alt=\"Colors compared in CIEDE2000\" align=left>
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-a4-ciede2000.png\"
     alt=\"4-frame average\" align=left>
<br>
CIE L*a*b* based extremely complicated formula called CIEDE2000.
<br>
And it seems to be worth its mathematics; the result looks really impeckable.
Well, except for the fact that yellows, such as in the bottom of the circle,
look greenish here. (In that specific part, RGB did almost perfectly.)

<br clear=all>

", 'cie_cmc:1.1. CMC l:c' => "

<img width=256 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/deltae_cmc.png\"
     alt=\"Colors compared in CMC\" align=left>
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-cmc.gif\"
     alt=\"Colors compared in CMC\" align=left>
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-a4-cmc.png\"
     alt=\"4-frame average\" align=left>
<br>
<br>
CIE L*a*b* based quite complicated formula called CMC l:c, with l=1.5 and c=1.0.
<br><br>
Interestingly, this operator seemed to have a huge issue with black colors;
animmerger has a special workaround for that problem, though the darker
green region looks weird too. In general, this has the appearance of being
the weakest of all of these operators. The averaged picture has wrong colors
here and there. Use it only if you are looking for a specific type of special
effect.

<br clear=all>

", 'cie_bfd:1.1. BFD l:c' => "

<img width=256 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/deltae_bfd.png\"
     alt=\"Colors compared in BFD\" align=left>
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-bfd.gif\"
     alt=\"Colors compared in BFD\" align=left>
<img width=400 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/tk-a4-bfd.png\"
     alt=\"4-frame average\" align=left>
<br>
CIE L*a*b* based extremely complicated formula called BFD l:c, with l=1.5 and c=1.0.

<br clear=all>

", 'cie_illuminants:1.1. Illuminants' => "

RGB to LAB conversions are subject to a lot of perception based science.
A concept of \"illuminant matrix\" plays a significant role here.
<p>
Animmerger knows three illuminant matrices:
<table><tr>
  <td>CIE&nbsp;E illuminant:<br>
<pre style=\"margin:0\">0.488718, 0.176204,  0.000000
0.310680, 0.812985,  0.0102048
0.200602, 0.0108109, 0.989795</pre></td>
  <td style=\"border-left:1px dashed #888\"
    >Adobe&nbsp;D65 illuminant:<br>
<pre style=\"margin:0\">0.576700, 0.297361,  0.0270328
0.185556, 0.627355,  0.0706879
0.188212, 0.0752847, 0.99124</pre></td>
  <td style=\"border-left:1px dashed #888\"
    >Unidentified D65-based illuminant, found in Imagemagick:<br>
<pre style=\"margin:0\">0.412453, 0.357580, 0.180423
0.212671, 0.715160, 0.072169
0.019334, 0.119193, 0.950227</pre></td>
</tr></table>
 <p>
Animmerger uses illuminant #3 for CIE76, and illuminant #1 for all other
CIE based compare methods, because illuminants #2 and #3 have serious
issues with blue and purple tones when any other compare method than CIE76 is used
(specifically, they suggest that black is the overwhelmingly best substitute for those colors).
 <p>
Animmerger converts a RGB value into
CIE&nbsp;L*a*b* and CIE&nbsp;L*C*h*
using the following formula:
<code>
<br> X = (i0 × R + i3 × G + i6 × B) ÷ 255 ÷ (i0+i1+i2)
<br> Y = (i1 × G + i4 × G + i7 × B) ÷ 255 ÷ (i3+i4+i5)
<br> Z = (i2 × B + i5 × G + i8 × B) ÷ 255 ÷ (i6+i7+i8)
<br> f(v) = v &le; 6<sup>3</sup>29<sup>&minus;3</sup> ? 4÷29 + v × 29<sup>2</sup>6<sup>&minus;2</sup>3<sup>&minus;1</sup> :  v<sup>1÷3</sup>
<br> L = 4×29 × f(Y) - 4<sup>2</sup>
<br> a = 500 × (f(X) - f(Y))
<br> b = 200 × (f(Y) - f(Z))
<br> C = √(a² + b²)
<br> h = atan2(b, a)</code><br>
Where i0&hellip;i8 are the values from the illuminant matrix.

", 'transformation:1. Transformation' => "

Mathematical transformations can be applied to individual pixels
of the resulting image, using the <code>--transform</code> option.
 <p>
In this example, the overall color tone of the image was changed
and a lens flare effect was added:<br>
<img width=724 height=224
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/trans.gif\"
     alt=\"Transformed with a flare effect\"><br>
<p>
Produced with:<br>
<code>
# ./animmerger --gif snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF \\<br>
# &nbsp; &nbsp; &nbsp; &nbsp; -pv -l25 --cie -Qd,64 -Qd,32 --dr 0 --dc 16 \\<br>
# &nbsp; &nbsp; &nbsp; &nbsp; --transform \"luma:=r*.299+g*.587+b*.114; pi:=atan2(0,-1);px:=238;py:=135;\" \\<br>
# &nbsp; &nbsp; &nbsp; &nbsp; --transform \"xy:=(abs(x-px)*abs(y-py)/200^2);\" \\<br>
# &nbsp; &nbsp; &nbsp; &nbsp; --transform \"rad:=hypot(x-px,y-py)^1.2;\" \\<br>
# &nbsp; &nbsp; &nbsp; &nbsp; --transform \"v:=luma+(300-min(300,rad))+(200-min(300,(xy+1e-6)^0.6*7e3));\" \\<br>
# &nbsp; &nbsp; &nbsp; &nbsp; --transform r=\"r*v/(255*0.299*2.2)\" \\<br>
# &nbsp; &nbsp; &nbsp; &nbsp; --transform g=\"g*pow(v,1.2)/(255*0.587*3.1)\" \\<br>
# &nbsp; &nbsp; &nbsp; &nbsp; --transform b=\"b*v/(255*0.114*1.7)+50*(1.35+cos(1+(y+x*.2)*pi/100))\" -vv<br>
# gifsicle -O2 -o demo/trans.gif -d3 -l0 tile-????.gif</code>

", 'caveats:1. Caveats' => "

", 'caveat_parallax:1.1. Parallax motion' => "

Parallax motion is bad. When animating video game content, please ensure that
all background layers are synchronized. Note that this will likely require you
to hack the emulator that is used to produce the video frames.
<p>
If different background layers are moving at different speeds with respect
to the camera, animmerger will sync into one of them (probably the one that
occupies the largest screen area), and the rest will appear to be moving with
respect to the chosen background.
 <p>
Example:<br>
<table border=1>
 <tr>
  <td colspan=2>
This scene is from Super Mario World. The HUD layer is disabled,
but otherwise it is an intact animation. The palette was reduced
and fps halved to make the file slightly smaller for web distribution.
  </td>
 </tr>
 <tr valign=top>
  <td width=\"480\">
<img width=480 height=400
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/pano5-cl.gif\"
     alt=\"Super Mario World with parallax motion\">
  </td>
  <td width=\"480\">
<img width=480 height=400
     src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/pano5-fl.gif\"
     alt=\"Super Mario World with parallax fix\"><br>
  </td>
 </tr>
 <tr valign=top>
  <td>
You can easily see the problem with parallax motion: Sometimes
the image alignment syncs to the platforms, sometimes it syncs
to the stalactite background. When it syncs to the platforms, the
other background can be seeing moving as a distinct layer.
</td>
  <td>
In this version, the background layers move in unison with respect
to the camera. As such, the image alignment is perfect.<br>
This was achieved with the following patch to snes9x:
<pre style=\"font-size:84%;height:6em;overflow:auto;border:1px solid #444;padding:1px\"
>--- ppu.cpp~    2010-08-17 23:46:11.022843689 +0300
+++ ppu.cpp     2010-08-17 22:34:52.000000000 +0300
@@ -416,2 +416,3 @@
                        PPU.BG[0].HOffset = (Byte<<8) | PPU.BGnxOFSbyte;
+                       PPU.BG[1].HOffset = (Byte<<8) | PPU.BGnxOFSbyte;
                        PPU.BGnxOFSbyte = Byte;
@@ -423,2 +424,3 @@
                        PPU.BG[0].VOffset = (Byte<<8) | PPU.BGnxOFSbyte;
+                       PPU.BG[1].VOffset = (Byte<<8) | PPU.BGnxOFSbyte;
                        PPU.BGnxOFSbyte = Byte;
@@ -429,3 +431,3 @@
                  case 0x210F:
-                       PPU.BG[1].HOffset = (Byte<<8) | PPU.BGnxOFSbyte;
+                       //PPU.BG[1].HOffset = (Byte<<8) | PPU.BGnxOFSbyte;
                        PPU.BGnxOFSbyte = Byte;
@@ -436,3 +438,3 @@
                  case 0x2110:
-                       PPU.BG[1].VOffset = (Byte<<8) | PPU.BGnxOFSbyte;
+                       //PPU.BG[1].VOffset = (Byte<<8) | PPU.BGnxOFSbyte;
                        PPU.BGnxOFSbyte = Byte;</pre>
</td></tr>
</table>

The commands used to produce these animations were:<br>
<code># rm tile-*.gif;  animmerger -v -r12x12 -bl -pc -a -4,-3,6,9 pano5/*.png<br>
# rm tile-???[13579].gif   # Delete 50% of frames to reduce fps in half<br>
# gifsicle -O2 -o demo/pano5-cl.gif --crop 8,8+480x400 --use-colormap smwpalette.gif -l0 -d6 tile-????.gif<br>
# rm tile-*.gif; animmerger -v -r12x12 -bl -pc -a -4,-3,6,9 pano5b/*.png<br>
# rm tile-???[13579].gif   # Delete 50% of frames to reduce fps in half<br>
# gifsicle -O2 -o demo/pano5-fl.gif --crop 8,8+480x400 --use-colormap smwpalette.gif -l0 -d6 tile-????.gif
</code>
<p>
The palette file was customized by hand, by taking a representative
snapshot of the movie, and then progressively merging near-identical
entries in the colormap in GIMP manually until only the minimal
set of unique colors/tones remain.

", 'caveat_flash:1.1. Flashes, fog and other transparent layers' => "

The image aligning engine is confused by anything that
globally changes the screen brightness.  This includes
any pain-red-tinting, white-explosion flashes, fog clouds,
etc. Please try to avoid them.
 <p>
Example:<br>
<i>TODO: Add successful Super Metroid animation</i>
<p>
<i>TODO: Add example of how image alignment suffers
when using the power bomb in Super Metroid</i>

", 'usage:1. Usage' => "

<pre class=smallerpre>".htmlspecialchars(usagetext('animmerger'))."</pre>

", '1. Copying' => "

animmerger has been written by Joel Yliluoma, a.k.a.
<a href=\"http://iki.fi/bisqwit/\">Bisqwit</a>,<br>
and is distributed under the terms of the
<a href=\"http://www.gnu.org/licenses/licenses.html#GPL\">General Public License</a> (GPL).

", '1. Requirements' => "

GNU make and C++ compiler is required to recompile the source code.<br>
libgd is also required.

", '1. See also' => "

<ul>
<li><a href=\"http://advsys.net/ken/utils.htm\">pngout</a> for making PNG files considerably smaller.
</li>
<li><a href=\"http://www.lcdf.org/gifsicle/\">gifsicle</a> for creating and optimizing GIF animations
</li>
</ul>

");
include '/WWW/progdesc.php';
