Classes :

- Keybinds
- Scene
- LightSource (PonctualLightSource, DirectionalLightSource)

Render pass :

```
foreach shader
    set shader
    foreach material using that shader
        set uniforms, textures
        foreach object using that material
             draw
```
