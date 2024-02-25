Note: This was from a base code provided to me. I, however had some problems with git and didn't get a lot of time to work on this, so I somehow pushed everything and some things might not look right in regards to the initial commit.

Assumptions: The lights have Anti-Aliasing off. This was done to speed up the program. If required, I have to just move a couple of lines here and there to turn on the anti-aliasing of the light source.

-- For assignment submission commit --

All the final images are in the `output_scenes_final` folder and the final report is in the same folder named as `report_final.md`.

# Introduction to Computer Graphics (CS7.302, IIIT Hyderabad)
This is the base code that is to be used for all assignments in the above course. <br>
Some sample scenes are given in the <a href="https://github.com/cs7-302-graphics/scenes">cs7-302-graphics/scenes</a> repo.

Instructions to clone, compile and run are given below.

## Cloning this repo
```git clone --recursive https://github.com/cs7-302-graphics/simple_renderer```
Do not forget the `--recursive` flag!

## Compiling
```
mkdir build
cd build
cmake ..
```

If you are on windows, this should create a Visual Studio solution ```cs7302.sln``` in the build folder. Open it and compile. \
If you are on linux/mac, you will need to additionally run the following to compile:

```
make -j8
```

## Running
The path to scene config (typically named `config.json`) and the path of the output image are passed using command line arguments as follows:
```bash
./build/render <scene_path> <out_path>
```
