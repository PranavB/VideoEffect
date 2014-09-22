﻿using Nokia.Graphics.Imaging;
using System;
using System.Threading.Tasks;
using VideoEffects;
using Windows.Storage;
using Windows.Storage.Streams;

namespace UnitTests
{
    public enum EffectType
    {
        Lumia,
        ShaderNv12,
        ShaderBgrx8
    }

    class Utils
    {
        internal static async Task<IVideoEffectDefinition> CreateEffectDefinitionAsync(EffectType effectType)
        {
            switch (effectType)
            {
                case EffectType.Lumia:
                    return new LumiaEffectDefinition(() =>
                    {
                        return new IFilter[]
                    {
                        new AntiqueFilter(),
                        new FlipFilter(FlipMode.Horizontal)
                    };
                    });

                case EffectType.ShaderNv12:
                    IBuffer shaderY = await PathIO.ReadBufferAsync("ms-appx:///Invert_093_NV12_Y.cso");
                    IBuffer shaderUV = await PathIO.ReadBufferAsync("ms-appx:///Invert_093_NV12_UV.cso");
                    return new VideoEffects.ShaderEffectDefinition(shaderY, shaderUV);

                case EffectType.ShaderBgrx8:
                    IBuffer shader = await PathIO.ReadBufferAsync("ms-appx:///Invert_093_RGB32.cso");
                    return new VideoEffects.ShaderEffectDefinition(shader);

                default:
                    throw new ArgumentException("Invalid effect type");
            }
        }
    }
}