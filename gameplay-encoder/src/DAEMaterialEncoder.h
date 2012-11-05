#ifndef DAEMATERIALENCODER_H_
#define DAEMATERIALENCODER_H_

#include "Base.h"

namespace gameplay
{
    class EncoderArguments;
    class Material;
    class Effect;
    
    class DAEMaterialEncoder
    {
    public:
        /**
         * Constructor.
         */
        DAEMaterialEncoder(void);
        
        /**
         * Destructor.
         */
        virtual ~DAEMaterialEncoder(void);
        
        void processMaterial(const EncoderArguments& arguments, domCOLLADA* dom);
        
    private:
        enum DAEMaterial {
            EMISSION,
            AMBIENT,
            DIFFUSE,
            SPECULAR
        };
        
        void processEffect(domEffect *effect, Material *material);
        void processProfileCOMMON(domProfile_COMMON *pc, Material *material);
        bool processColorOrTextureType(domCommon_color_or_texture_type *cot,
                                       DAEMaterial channel,
                                       Effect &effect,
                                       domCommon_float_or_param_type *fop = NULL);
        
        bool getFloat4Param(xsNCName Reference, domFloat4 &f4);
        bool getFloatParam(xsNCName Reference, domFloat &f) const;
        bool processTexture(domCommon_color_or_texture_type_complexType::domTexture *tex, Effect &effect);
        
    private:
        domInstance_effectRef currentInstance_effect;
        domEffect *currentEffect;
        domCOLLADA* dom;
        
        std::list<Material*> materials;
    };
    
}

#endif
