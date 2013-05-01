## v1.7.0

- Adds -m parameter to encoder to support exporting Material from FBX scenes.
- Adds encoder animation grouping arguments for auto grouping and disabling grouping via -g:auto and -g:off
- Adds UI ImageControl
- Adds UI control events for enter and leave.
- Adds UI and theme support for focus state on controls.
- Adds Gamepad support in UI.
- Adds smooth interial mouse scrolling.
- Adds keyboard event handling code that was missed on some UI containers and controls.
- Adds Game accelerometer and gyro improvements.
- Adds XBox 360 controller support to MacOS X.
- Adds RenderState enum for supporting cull side definition for front, back and front-back culling.
- Adds a lua function "convert(object, className)" that will convert a gameplay userdata object to another class type by changing the metatable. (For example: This lets you convert Control to Button in lua)
- Adds gesture events to script.
- Adds script friendly verions of Camera:project(..)
- Adds support for defining lights in .scene file.
- Adds MaterialParameter setter that are script friendly.
- Adds methods to FrameBuffer for querying render target info.
- Adds FileSystem::getDirectoryName()
- Adds support so that Properties can be loaded relative to .property files.
- Adds methods on ScriptController for registering and unregisering global script callback functions.
- Adds Game::resizeEvent(..)
- Adds Game::getArguments()
- Adds Scene::load() so it can load from ".gpb" files.
- Adds some additional operators overloads for Vector classes.
- Adds RenderState::clearParameter method to allow existing material parameter values/bindings to be cleared.
- Adds loopBlendTime property for animation clips to support interpolating between the end points of an animation clip when looping.
- Fixes Xcode support to be up to date to version 6.1
- Fixes Form::projectPoint for forms that are scaled.
- Fixes Bullet NEON build problems.
- Fixes problem with static rigid bodies attached to nodes.
- Fixes problems with ghost objects and kinimatic rigid bodies.
- Fixes userData pointer that was not cloned properly when cloning a node.
- Fixes bug in createChannel with keyCount = 1.
- Fixes encoder to ignore 1 frame animations caused by FBX export on Blender.
- Fixes buggy tab order in UI.
- Fixes touch event problems not propagating up on mobile touch platforms.
- Fixes parent relationship on cloned Technique and Pass.
- Fixes Node to include light contribution for bounding volume computations.
- Fixed issue where multiple samplers pointing to the same texture would share the same state instead of using sampler-specific filter and wrap state.
- Fixes calculation of the transform matrix in FBX encoder for lights properly considering vectors like pre and post rotation scenarios.
- Fixes blurry borders on UI forms by using proper texture filtering.
- Fixes lua dofile on Windows.
- Fixes in colored shader when using spot ligts.
- Fixes issue in encoder which was converting point lights into ambient lights when they had no decay.
- Fixes to CMake projects on different platforms.
- Cleans up Platform by moving commong platform code to Platform.cpp
- Cleans up and changes Gamepad APIs.
- Cleans repo directory restructure dropping gameplay- prefix on many folders and files.
- Removes bin and external-deps folder/contents to external server with new install scripts that are now required to be run after cloning the repo.
- Removes pre-compiled api/html docs to reduce repository bloat.
- Removes support for COLLADA DAE and now support FBX.
- Removes middle mouse button simulated desktop accelerometer.

## v1.6.0

- Adds file Stream interface for reading/writing files instead of using fread/fwrite. 
- Adds Terrain class to support for heightmap based terrains featuring LOD, multiple surface layers, loading from PNG, RAW8/16, full transform, physics, patch culling and verticle skirt for cracks.
- Adds object-space normal map generation to gameplay-encoder for terrain normal map generation.
- Adds scene support for loading .terrain files in .scene files.
- Adds scene support for inline cameras to .scene files.
- Adds suppoft for defining .scene files without 'path' to gpb. New node can not be create in .scene file.
- Adds static Scene::getScene(const char*) to query currently active scenes in a game, helpful for script access.
- Adds support for multiple translate, rotate and scale commands in a single node entity within .scene files, processed in-order they are defined.
- Adds scene support for material auto binding scene ambient color, light color and light direction.
- Adds support for setting the depth compare function on materials.
- Adds support for texture/sampler arrays being passed to materials.
- Adds support for loading uncompressed DDS textures for the following formats: R8G8B8, A8R8G8B8, A8B8G8R8, X8R8G8B8, X8B8G8R8
- Adds improvments to prefer higher quality mipmap generation.
- Adds improved Gamepad API support for button enumeration, triggers and some mobile Gamepad support on BlackBerry.
- Adds additional gameplay-tests for billboards, forms, gamepads and lights.
- Adds support for launching the browser via launchURL(const char*).
- Adds physics support for setLinearFactor and setAngularFactor  on rigid bodies.
- Adds methods to PhysicsCollisionObject to allow conversion to subclass types (i.e. PhysicsRigidBody, PhysicsCharacter, etc) from script.
- Adds option for fullscreen without width/height config to use native desktop resolution.
- Adds Linux support for OpenAL PulseAudio back-end.
- Adds support for latest Bullet Physics 2.81 with NEON optimizations for mobile targets.
- Adds support for preprocessor directive NO_LUA_BINDINGS in the gameplay project to omit inclusion of generated lua bindings in compilation for developer mode value. 
- Adds optimizations to Lua generator to only write generated files if they differ from existing files, reducing both build times and committing of unchanged script binding files.
- Adds changes to Slider for setValueTextVisible, setValueTextAlignment, setValueTextPrecision and getters.
- Adds Microsoft Windows 7 64-bit support.
- Adds Apple iOS 6 support.
- Fixes to external-deps to reduce the size of the libraries on Windows.
- Fixes for Android to no longer need to copy files to the SD card before reading them. None of the Android samples require an SD card.
- Fixes for animation of opacity on UI and fonts.
- Fixes in UI for removing controls and also setVisible(bool).
- Fixes for UI controls missing on MacOSX.
- Fixes for setting UI alignment programmatically.
- Fixes for lighting shaders.
- Fixes to the texture minification mode from GL_LINEAR_MIPMAP_LINEAR to GL_NEAREST_MIPMAP_LINEAR for newly created textures with mipmaps.
- Fixes minor memory leaks and possible access violations when calling Game::exit() from script.
- Fixes physics debug drawing for large scenes causing the internal MeshBatch to grow to an enormous size.

## v1.5.0

- Linux support. (tested on Ubuntu 12)
- CMake support for makefile generation for Linux.
- Gamepad API support for desktops.
- Touch gesture support for tap, swipe and pinch.
- Vehicle physics support via new PhysicsVehicle and PhysicsVehicleWheel classes.
- Adds new racer sample (sample06-racer).
- Adds gameplay-tests project as a test app for various basic engine features with some initial tests.
- Adds support for Scene files for wildcard identifiers.
- Adds Visual Studio Plug-in support for BlackBerry PlayBook and BlackBerry 10.
- Adds configurable multi-sampling anti-aliasing support. 
- Adds updates to latest FBX SDK 2013.3.
- Adds file formats documentation for game.config .scene, .material, .animation, .physics, .particle
- Adds Game/Platform::canExit for testing device capabilities to quit. (only ios)
- Web community forums at http://www.gameplay3d.org/forums.
- Changed keyTimes from unsigned long[]  to unsigned int[]. (breaks compat. in AnimationTarget and Animation::Channel)
- Fixes inconsistencies from Bundle::getObjectID() to Bundle::getObjectId() (breaks compat. in Bundle)
- Fixes the texture coordinates of Mesh::createQuad(float x, float y, float width, float height).
- Fixes line-wise distortion when loading RGB png's into textures that are non-power of two.
- Fixes inconsistencies in createXXXX methods.  (breaks compat. in Scene)
- Fixes Rectangle::contains.
- Fixes Lua print logging.
- Fixes Lua errors to be treated as runtime warnings.
- Fixes setVertexData to pointers instead of constant data.
- Fixes AudioSource so that it doesn't loop by default.
- Fixes minor UI scrolling issues.

## v1.4.0

- Lua script bindings for all gameplay interfaces.
- Lua script binding generator tool (gameplay-luagen) for generating gameplay Lua bindings from doxygen xml output.
- AIController, AIAgent, AIStateMachine, AIState and AIMessage classes for scripted AI support.
- Sample for sample05-lua to demonstrate basic Lua with AI scripting.
- Gamepad class with virtual gamepad support.
- Pre-built versions gameplay-encoder added to bin folder with TTF, DAE and FBX support built-in.
- Improved modular shaders with support for #include in shaders. (breaks compat. for shaders)
- LightMap support into colored-unlit.frag and textured-unlit.frag shaders.
- Adds cloning and wireframing features to sample03-character.
- Adds kick the ball on the sample03-character to demonstrate 2 buttons and more physics.
- Fixes missing mouse events on UI controls.
- Fixes to gameplay-encoder to prompt user for font size if not specified.
- Fixes to add "-g" as short form argument for grouping animations.
- Fixes node cloning.
- Fixes to gameplay-encoder for output file path when encoding fonts.
- Fixes to FrameBuffer, RenderTarget and DepthStencilTarget.
- Fixes user switching in MacOSX to other applications with Apple-Tab.
- Fixes measureText with empty string to be proper size.
- Fixes for aliased text by applying linear filtering by default on Fonts.
- Fixes RenderState::StateBlock::bindNoRestore() issue where blend function was not restored to the proper defaults.
- Fixes some inconsistencies in Game event method names for menuEvent. (breaks compat. in Game)
- Fixes some inconsistencies with AnimationClip::getID() to be same as Node::getId() and other classes. (breaks compat. in AnimationClip)
- Fixes to gameplay-encoder and PhysicsRigidBody for heightmaps which adds height precision into generated heightmap png's.

## v1.3.0

- Portrait mode games on mobile platforms.
- Fullscreen and configurable game resolutions on desktop platforms.
- User Interface support for scrolling with scrollbars on Container.
- PVRTC, ATC and DXT texture compression support.
- Performance improvements in user interface forms and text.
- Performance improvements in animations on transforms.
- Performance improvements using NEON math for BlackBerry and iOS.
- Fixes for improvements in error handling throughout all systems.
- Fixes supporting built-in Maya COLLADA exporter via DAE_FBX export.
- Fixes for latest FBX SDK 2013 support.
- Fixes for loading from some WAV files that were crashing.
- Fixes for From/By animations.
- Fixes allowing all inline properties loaded within .scene files. (breaks compat. for .scene)
- Fixes in .scene files for collisionObject definitions (breaks compat. for .scene)
- Fixes for depth/z-ordering of controls.

## v1.2.0

- BlackBerry 10 support.
- iOS 5.1 support.
- Android 2.3+ support.
- User interface system with declarative forms and themes.
- Bluetooth keyboard/mouse support on BlackBerry platform.
- Developer guide.
- Sample/tutorial for sample03-character.
- Sample for sample04-particles to demonstrate particle emitters.
- Fixes for loading properties from URL.
- Fixes on Win32/MacOSX for when mouse pointer leaves the window and returns.
- Fixes to accelerometer for Android.
- Fixes in animation blending.
- Fixes to GPB for loading from single node and parent node. (breaks compat. for .gpb)

## v1.1.0

- FBX support in gameplay-encoder.
- MacOSX platform support using XCode.
- Off-screen rendering functionality using FrameBuffer.
- Loading 3D scenes using declarative  .scene files.
- Loading audio from .ogg files using vorbis.
- Loading AudioSources from .audio files.
- Loading Animations from .animation files.
- AnimationClip support for cross fading.
- Physics support using Bullet Physics.
- Cross-platform new project generator.
- Overloaded operators in Math classes.
- Font improvements for justify, clip, wrap and scaling.
- Fixes for Font::drawText to use point size and not float scalar.
- Fixes for memory leaks in and fixes to AnimationTarget.
- Fixes for bumped and parallax shaders.
- Fixes to simplify folders for resources in samples.
- Fixes to the material/shader system.
- Fixes to the ParticleEmitter.

## v1.0.0

- Initial release.




