## v1.4.0

- Pre-built versions gameplay-encoder added to bin folder with TTF, DAE and FBX support built-in.
- Improved modular shader library with support for #include in shaders.
- Adds Gamepad class for virtual gamepad support.
- Adds the ability for cloning and wireframe the boy in sample03-character.
- Fixes to gameplay-encoder to prompt user for font size if not specified.
- Fixes to add "-g" as short form argument for grouping animations.
- Fixes node cloning.
- Fixes to gameplay-encoder for output file path when encoding fonts.
- Fixes to FrameBuffer, RenderTarget and DepthStencilTarget.


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
- User interface system with declaritive forms and themes.
- Bluetooth keyboard/mouse support on BlackBerry platform.
- Developer guide.
- Sample/turorial for sample03-character.
- Sample for sample04-particles.
- Fixes for loading properties from URL.
- Fixes on Win32/MacOSX for when mouse pointer leaves the window and returns.
- Fixes to accelerometer for Android.
- Fixes in animation blending.
- Fixes to GPB for loading from single node and parent node. (breaks compat. for .gpb)

## v1.1.0

- FBX support in gameplay-encoder.
- MacOSX platform support using XCode.
- Offscreen rendering functionality using FrameBuffer.
- Loading 3D scences using declaritive .scene files.
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
- Fixes for bumped and paralax shaders.
- Fixes to simplify folders for resources in samples.
- Fixes to the material/shader system.
- Fixes to the ParticleEmitter.

## v1.0.1

- Initial release.




