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

Animmerger converts a 2D animation from local-frame-of-reference
into global-frame-of-reference. That is, for a movie that follows
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
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-a.png\"
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
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-t.png\"
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
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-m.png\"
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

The \"last\" method is a simpler implementation of the \"most used\"
method, simply recording the last pixel value in any location.
<p>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-l.png\"
     alt=\"Last\">
<p>
Produced with commandline:<br>
<code># animmerger -pl snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# mv tile-0000.png demo/method-l.png</code>

", 'last:1.1.1. FIRST'=> "

The \"first\" method is analogous to \"last\".
It shows whatever first appeared in a particular pixel location.
<p>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-f.png\"
     alt=\"First\">
<p>
The turtles are distorted, because they moved while the screen scrolled.<br>
It is the same effect as if you move the paper in a desktop scanner during the scanning.
<p>
Produced with commandline:<br>
<code># animmerger -pf snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# mv tile-0000.png demo/method-f.png</code>

", 'firstnmost:1.1.1. FIRSTNMOST'=> "

The \"firstnmost\" method is analogous to \"first\" and \"mostused\";
it chooses the most common pixel of first N pixel values.
Set N with the --firstlast (-f) option.<br>
If N is 0, instead gets last uncommon pixel.<br>
If N is negative, using least common values rather than most common.
<p>
Most common of first 4:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Ff4.png\"
     alt=\"Most common of first 4\">
<p>
Most common of first 10:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Ff10.png\"
     alt=\"Most common of first 10\">
<p>
Most common of first 16:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Ff16.png\"
     alt=\"Most common of first 16\">
<p>
First uncommon:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Ff0.png\"
     alt=\"First uncommon\">
<p>
Least common of first 10:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Ff-10.png\"
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
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Lf10.png\"
     alt=\"Most common of last 10\">
<p>
Last uncommon:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Lf0.png\"
     alt=\"Last uncommon\">
<p>
Least common of last 10:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-Lf-10.png\"
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
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-e.png\"
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
<p>
Produced with commandline:<br>
<code>
# rm tile-*.png tile-*.gif<br>
# animmerger --gif -pc snaps/*.png -m0,8,256,16,020202,A64010,D09030,006E84,511800,FFFFFF<br>
# <a href=\"http://www.lcdf.org/gifsicle/\">gifsicle</a> -O2 -o demo/method-c.gif -l0 -d3 tile-*.gif</code><br>
The version with HUD intact was created with the same commandline,
except with the -m option removed.

", 'motionblur:1.1.1.1. Motion blur' => "

The changelog method also supports motion blur. Use the --motionblur (-B) option to set it.
Value 0 disables motion blur (default: 0).
<p>
Blur length 1:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-cB1.gif\"
     alt=\"Changelog, blur 1\">
<p>
Blur length 4:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-cB4.gif\"
     alt=\"Changelog, blur 4\">
<p>
Blur length 20:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-cB20.gif\"
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
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-sl30.gif\"
     alt=\"Loop, 30 frames\">
<p>
10 frames (66738 bytes):<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-sl10.gif\"
     alt=\"Loop, 10 frames\">
<p>
4 frames (40372 bytes):<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-sl4.gif\"
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
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl30.gif\"
     alt=\"Loop, 30 frames\">
<p>
10 frames:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl10.gif\"
     alt=\"Loop, 10 frames\">
<p>
4 frames:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl4.gif\"
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
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl30B20.gif\"
     alt=\"Loop-Avg 30, blur 8\">
<p>
Loop length 30 frames, blur length 20, with YUV calculations:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl30yB20.gif\"
     alt=\"Loop-Avg 30 through YUV, blur 8\">
<p>
Loop length 10 frames, blur length 4:<br>
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/method-vl10B4.gif\"
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
 </tr><tr><th>MostUsed</th>               <td>Static          </td>   <td>No</td>     <td>No       </td><td>12&hellip;16 + 6*number of unique colors</td>   <td>Maps</td>
 </tr><tr><th>LeastUsed</th>              <td>Static          </td>   <td>No</td>     <td>No       </td><td>As MostUsed</td>                                <td></td>
 </tr><tr><th>Average</th>                <td>Static          </td>   <td>Yes</td>    <td>Yes      </td><td>16</td>                                         <td></td>
 </tr><tr><th>Tinyaverage</th>            <td>Static          </td>   <td>Yes</td>    <td>No       </td><td>8</td>                                          <td></td>
 </tr><tr><th>ActionAvg</th>              <td>Static          </td>   <td>Yes</td>    <td>Yes      </td><td>As MostUsed</td>                                <td></td>
 </tr><tr><th>ChangeLog</th>              <td>Animated (movie)</td> <td>If blur</td><td>For blur </td><td>12&hellip;16 + 8*number of content changes</td> <td>Animations</td>
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
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/pano5-cl.gif\"
     alt=\"Super Mario World with parallax motion\">
  </td>
  <td width=\"480\">
<img src=\"http://bisqwit.iki.fi/jutut/kuvat/animmerger/pano5-fl.gif\"
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
