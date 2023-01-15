# HaloInfNVAPI
A CLI tool to configure specific settings within Halo Infinite's NVIDIA Driver Profile.

# Usage
```
HaloInfNVAPI.exe <Option> <Value>
- <Option>: <Values>
- FPS: 20 - 1000
- LODBias: Off/Low/Medium/High
```

1. FPS    
    Valid values range from 20 to 1000 FPS.     
    The maximum framerate option is altered.    
  
2. LODBias  
    This value uses a preset based system.    
    This option alters the driver profile's LOD Bias (DX) value which controls the level of detail within the game. (This only affects the game's textures.)   
    - 0
    - -1
    - -1.5
    - -2
    - -2.5
    - -3

# Build
1. Open the Solution file in the latest version of Visual Studio.
2. Build the Solution.
