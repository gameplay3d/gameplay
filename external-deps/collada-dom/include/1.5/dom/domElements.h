#ifndef __DOM_ELEMENTS_H__
#define __DOM_ELEMENTS_H__

#include <dom/domTypes.h>

class domInput_global;

typedef daeSmartRef<domInput_global> domInput_globalRef;
typedef daeTArray<domInput_globalRef> domInput_global_Array;

class domInput_local;

typedef daeSmartRef<domInput_local> domInput_localRef;
typedef daeTArray<domInput_localRef> domInput_local_Array;

class domInput_local_offset;

typedef daeSmartRef<domInput_local_offset> domInput_local_offsetRef;
typedef daeTArray<domInput_local_offsetRef> domInput_local_offset_Array;

class domTargetable_float;

typedef daeSmartRef<domTargetable_float> domTargetable_floatRef;
typedef daeTArray<domTargetable_floatRef> domTargetable_float_Array;

class domTargetable_float3;

typedef daeSmartRef<domTargetable_float3> domTargetable_float3Ref;
typedef daeTArray<domTargetable_float3Ref> domTargetable_float3_Array;

class domToken_array;

typedef daeSmartRef<domToken_array> domToken_arrayRef;
typedef daeTArray<domToken_arrayRef> domToken_array_Array;

class domIdref_array;

typedef daeSmartRef<domIdref_array> domIdref_arrayRef;
typedef daeTArray<domIdref_arrayRef> domIdref_array_Array;

class domName_array;

typedef daeSmartRef<domName_array> domName_arrayRef;
typedef daeTArray<domName_arrayRef> domName_array_Array;

class domBool_array;

typedef daeSmartRef<domBool_array> domBool_arrayRef;
typedef daeTArray<domBool_arrayRef> domBool_array_Array;

class domFloat_array;

typedef daeSmartRef<domFloat_array> domFloat_arrayRef;
typedef daeTArray<domFloat_arrayRef> domFloat_array_Array;

class domInt_array;

typedef daeSmartRef<domInt_array> domInt_arrayRef;
typedef daeTArray<domInt_arrayRef> domInt_array_Array;

class domParam;

typedef daeSmartRef<domParam> domParamRef;
typedef daeTArray<domParamRef> domParam_Array;

class domAccessor;

typedef daeSmartRef<domAccessor> domAccessorRef;
typedef daeTArray<domAccessorRef> domAccessor_Array;

class domP;

typedef daeSmartRef<domP> domPRef;
typedef daeTArray<domPRef> domP_Array;

class domLookat;

typedef daeSmartRef<domLookat> domLookatRef;
typedef daeTArray<domLookatRef> domLookat_Array;

class domMatrix;

typedef daeSmartRef<domMatrix> domMatrixRef;
typedef daeTArray<domMatrixRef> domMatrix_Array;

class domScale;

typedef daeSmartRef<domScale> domScaleRef;
typedef daeTArray<domScaleRef> domScale_Array;

class domSkew;

typedef daeSmartRef<domSkew> domSkewRef;
typedef daeTArray<domSkewRef> domSkew_Array;

class domTranslate;

typedef daeSmartRef<domTranslate> domTranslateRef;
typedef daeTArray<domTranslateRef> domTranslate_Array;

class domImage_source;

typedef daeSmartRef<domImage_source> domImage_sourceRef;
typedef daeTArray<domImage_sourceRef> domImage_source_Array;

class domImage_mips;

typedef daeSmartRef<domImage_mips> domImage_mipsRef;
typedef daeTArray<domImage_mipsRef> domImage_mips_Array;

class domChannel;

typedef daeSmartRef<domChannel> domChannelRef;
typedef daeTArray<domChannelRef> domChannel_Array;

class domSampler;

typedef daeSmartRef<domSampler> domSamplerRef;
typedef daeTArray<domSamplerRef> domSampler_Array;

class domTechnique;

typedef daeSmartRef<domTechnique> domTechniqueRef;
typedef daeTArray<domTechniqueRef> domTechnique_Array;

class domExtra;

typedef daeSmartRef<domExtra> domExtraRef;
typedef daeTArray<domExtraRef> domExtra_Array;

class domPolylist;

typedef daeSmartRef<domPolylist> domPolylistRef;
typedef daeTArray<domPolylistRef> domPolylist_Array;

class domPolygons;

typedef daeSmartRef<domPolygons> domPolygonsRef;
typedef daeTArray<domPolygonsRef> domPolygons_Array;

class domLinestrips;

typedef daeSmartRef<domLinestrips> domLinestripsRef;
typedef daeTArray<domLinestripsRef> domLinestrips_Array;

class domTristrips;

typedef daeSmartRef<domTristrips> domTristripsRef;
typedef daeTArray<domTristripsRef> domTristrips_Array;

class domLines;

typedef daeSmartRef<domLines> domLinesRef;
typedef daeTArray<domLinesRef> domLines_Array;

class domTriangles;

typedef daeSmartRef<domTriangles> domTrianglesRef;
typedef daeTArray<domTrianglesRef> domTriangles_Array;

class domTrifans;

typedef daeSmartRef<domTrifans> domTrifansRef;
typedef daeTArray<domTrifansRef> domTrifans_Array;

class domVertices;

typedef daeSmartRef<domVertices> domVerticesRef;
typedef daeTArray<domVerticesRef> domVertices_Array;

class domAsset;

typedef daeSmartRef<domAsset> domAssetRef;
typedef daeTArray<domAssetRef> domAsset_Array;

class domImage;

typedef daeSmartRef<domImage> domImageRef;
typedef daeTArray<domImageRef> domImage_Array;

class domCamera;

typedef daeSmartRef<domCamera> domCameraRef;
typedef daeTArray<domCameraRef> domCamera_Array;

class domLight;

typedef daeSmartRef<domLight> domLightRef;
typedef daeTArray<domLightRef> domLight_Array;

class domInstance_with_extra;

typedef daeSmartRef<domInstance_with_extra> domInstance_with_extraRef;
typedef daeTArray<domInstance_with_extraRef> domInstance_with_extra_Array;

class domInstance_image;

typedef daeSmartRef<domInstance_image> domInstance_imageRef;
typedef daeTArray<domInstance_imageRef> domInstance_image_Array;

class domInstance_camera;

typedef daeSmartRef<domInstance_camera> domInstance_cameraRef;
typedef daeTArray<domInstance_cameraRef> domInstance_camera_Array;

class domInstance_force_field;

typedef daeSmartRef<domInstance_force_field> domInstance_force_fieldRef;
typedef daeTArray<domInstance_force_fieldRef> domInstance_force_field_Array;

class domInstance_light;

typedef daeSmartRef<domInstance_light> domInstance_lightRef;
typedef daeTArray<domInstance_lightRef> domInstance_light_Array;

class domInstance_material;

typedef daeSmartRef<domInstance_material> domInstance_materialRef;
typedef daeTArray<domInstance_materialRef> domInstance_material_Array;

class domBind_material;

typedef daeSmartRef<domBind_material> domBind_materialRef;
typedef daeTArray<domBind_materialRef> domBind_material_Array;

class domInstance_controller;

typedef daeSmartRef<domInstance_controller> domInstance_controllerRef;
typedef daeTArray<domInstance_controllerRef> domInstance_controller_Array;

class domInstance_geometry;

typedef daeSmartRef<domInstance_geometry> domInstance_geometryRef;
typedef daeTArray<domInstance_geometryRef> domInstance_geometry_Array;

class domInstance_node;

typedef daeSmartRef<domInstance_node> domInstance_nodeRef;
typedef daeTArray<domInstance_nodeRef> domInstance_node_Array;

class domInstance_physics_material;

typedef daeSmartRef<domInstance_physics_material> domInstance_physics_materialRef;
typedef daeTArray<domInstance_physics_materialRef> domInstance_physics_material_Array;

class domInstance_rigid_constraint;

typedef daeSmartRef<domInstance_rigid_constraint> domInstance_rigid_constraintRef;
typedef daeTArray<domInstance_rigid_constraintRef> domInstance_rigid_constraint_Array;

class domLibrary_cameras;

typedef daeSmartRef<domLibrary_cameras> domLibrary_camerasRef;
typedef daeTArray<domLibrary_camerasRef> domLibrary_cameras_Array;

class domLibrary_images;

typedef daeSmartRef<domLibrary_images> domLibrary_imagesRef;
typedef daeTArray<domLibrary_imagesRef> domLibrary_images_Array;

class domLibrary_lights;

typedef daeSmartRef<domLibrary_lights> domLibrary_lightsRef;
typedef daeTArray<domLibrary_lightsRef> domLibrary_lights_Array;

class domInstance_effect;

typedef daeSmartRef<domInstance_effect> domInstance_effectRef;
typedef daeTArray<domInstance_effectRef> domInstance_effect_Array;

class domMaterial;

typedef daeSmartRef<domMaterial> domMaterialRef;
typedef daeTArray<domMaterialRef> domMaterial_Array;

class domLibrary_materials;

typedef daeSmartRef<domLibrary_materials> domLibrary_materialsRef;
typedef daeTArray<domLibrary_materialsRef> domLibrary_materials_Array;

class domFx_sampler;

typedef daeSmartRef<domFx_sampler> domFx_samplerRef;
typedef daeTArray<domFx_samplerRef> domFx_sampler_Array;

class domFx_sampler1D;

typedef daeSmartRef<domFx_sampler1D> domFx_sampler1DRef;
typedef daeTArray<domFx_sampler1DRef> domFx_sampler1D_Array;

class domFx_sampler2D;

typedef daeSmartRef<domFx_sampler2D> domFx_sampler2DRef;
typedef daeTArray<domFx_sampler2DRef> domFx_sampler2D_Array;

class domFx_sampler3D;

typedef daeSmartRef<domFx_sampler3D> domFx_sampler3DRef;
typedef daeTArray<domFx_sampler3DRef> domFx_sampler3D_Array;

class domFx_samplerCUBE;

typedef daeSmartRef<domFx_samplerCUBE> domFx_samplerCUBERef;
typedef daeTArray<domFx_samplerCUBERef> domFx_samplerCUBE_Array;

class domFx_samplerRECT;

typedef daeSmartRef<domFx_samplerRECT> domFx_samplerRECTRef;
typedef daeTArray<domFx_samplerRECTRef> domFx_samplerRECT_Array;

class domFx_samplerDEPTH;

typedef daeSmartRef<domFx_samplerDEPTH> domFx_samplerDEPTHRef;
typedef daeTArray<domFx_samplerDEPTHRef> domFx_samplerDEPTH_Array;

class domFx_rendertarget;

typedef daeSmartRef<domFx_rendertarget> domFx_rendertargetRef;
typedef daeTArray<domFx_rendertargetRef> domFx_rendertarget_Array;

class domFx_colortarget;

typedef daeSmartRef<domFx_colortarget> domFx_colortargetRef;
typedef daeTArray<domFx_colortargetRef> domFx_colortarget_Array;

class domFx_depthtarget;

typedef daeSmartRef<domFx_depthtarget> domFx_depthtargetRef;
typedef daeTArray<domFx_depthtargetRef> domFx_depthtarget_Array;

class domFx_stenciltarget;

typedef daeSmartRef<domFx_stenciltarget> domFx_stenciltargetRef;
typedef daeTArray<domFx_stenciltargetRef> domFx_stenciltarget_Array;

class domFx_clearcolor;

typedef daeSmartRef<domFx_clearcolor> domFx_clearcolorRef;
typedef daeTArray<domFx_clearcolorRef> domFx_clearcolor_Array;

class domFx_cleardepth;

typedef daeSmartRef<domFx_cleardepth> domFx_cleardepthRef;
typedef daeTArray<domFx_cleardepthRef> domFx_cleardepth_Array;

class domFx_clearstencil;

typedef daeSmartRef<domFx_clearstencil> domFx_clearstencilRef;
typedef daeTArray<domFx_clearstencilRef> domFx_clearstencil_Array;

class domFx_annotate;

typedef daeSmartRef<domFx_annotate> domFx_annotateRef;
typedef daeTArray<domFx_annotateRef> domFx_annotate_Array;

class domFx_newparam;

typedef daeSmartRef<domFx_newparam> domFx_newparamRef;
typedef daeTArray<domFx_newparamRef> domFx_newparam_Array;

class domFx_include;

typedef daeSmartRef<domFx_include> domFx_includeRef;
typedef daeTArray<domFx_includeRef> domFx_include_Array;

class domFx_code;

typedef daeSmartRef<domFx_code> domFx_codeRef;
typedef daeTArray<domFx_codeRef> domFx_code_Array;

class domFx_sources;

typedef daeSmartRef<domFx_sources> domFx_sourcesRef;
typedef daeTArray<domFx_sourcesRef> domFx_sources_Array;

class domFx_target;

typedef daeSmartRef<domFx_target> domFx_targetRef;
typedef daeTArray<domFx_targetRef> domFx_target_Array;

class domFx_common_float_or_param;

typedef daeSmartRef<domFx_common_float_or_param> domFx_common_float_or_paramRef;
typedef daeTArray<domFx_common_float_or_paramRef> domFx_common_float_or_param_Array;

class domFx_common_color_or_texture;

typedef daeSmartRef<domFx_common_color_or_texture> domFx_common_color_or_textureRef;
typedef daeTArray<domFx_common_color_or_textureRef> domFx_common_color_or_texture_Array;

class domFx_common_transparent;

typedef daeSmartRef<domFx_common_transparent> domFx_common_transparentRef;
typedef daeTArray<domFx_common_transparentRef> domFx_common_transparent_Array;

class domFx_common_newparam;

typedef daeSmartRef<domFx_common_newparam> domFx_common_newparamRef;
typedef daeTArray<domFx_common_newparamRef> domFx_common_newparam_Array;

class domProfile_common;

typedef daeSmartRef<domProfile_common> domProfile_commonRef;
typedef daeTArray<domProfile_commonRef> domProfile_common_Array;

class domProfile_bridge;

typedef daeSmartRef<domProfile_bridge> domProfile_bridgeRef;
typedef daeTArray<domProfile_bridgeRef> domProfile_bridge_Array;

class domGles2_newparam;

typedef daeSmartRef<domGles2_newparam> domGles2_newparamRef;
typedef daeTArray<domGles2_newparamRef> domGles2_newparam_Array;

class domGles2_shader;

typedef daeSmartRef<domGles2_shader> domGles2_shaderRef;
typedef daeTArray<domGles2_shaderRef> domGles2_shader_Array;

class domGles2_program;

typedef daeSmartRef<domGles2_program> domGles2_programRef;
typedef daeTArray<domGles2_programRef> domGles2_program_Array;

class domGles2_pass;

typedef daeSmartRef<domGles2_pass> domGles2_passRef;
typedef daeTArray<domGles2_passRef> domGles2_pass_Array;

class domProfile_gles2;

typedef daeSmartRef<domProfile_gles2> domProfile_gles2Ref;
typedef daeTArray<domProfile_gles2Ref> domProfile_gles2_Array;

class domGlsl_array;

typedef daeSmartRef<domGlsl_array> domGlsl_arrayRef;
typedef daeTArray<domGlsl_arrayRef> domGlsl_array_Array;

class domGlsl_newparam;

typedef daeSmartRef<domGlsl_newparam> domGlsl_newparamRef;
typedef daeTArray<domGlsl_newparamRef> domGlsl_newparam_Array;

class domGlsl_shader;

typedef daeSmartRef<domGlsl_shader> domGlsl_shaderRef;
typedef daeTArray<domGlsl_shaderRef> domGlsl_shader_Array;

class domGlsl_program;

typedef daeSmartRef<domGlsl_program> domGlsl_programRef;
typedef daeTArray<domGlsl_programRef> domGlsl_program_Array;

class domProfile_glsl;

typedef daeSmartRef<domProfile_glsl> domProfile_glslRef;
typedef daeTArray<domProfile_glslRef> domProfile_glsl_Array;

class domCg_array;

typedef daeSmartRef<domCg_array> domCg_arrayRef;
typedef daeTArray<domCg_arrayRef> domCg_array_Array;

class domCg_setparam;

typedef daeSmartRef<domCg_setparam> domCg_setparamRef;
typedef daeTArray<domCg_setparamRef> domCg_setparam_Array;

class domCg_user;

typedef daeSmartRef<domCg_user> domCg_userRef;
typedef daeTArray<domCg_userRef> domCg_user_Array;

class domCg_newparam;

typedef daeSmartRef<domCg_newparam> domCg_newparamRef;
typedef daeTArray<domCg_newparamRef> domCg_newparam_Array;

class domCg_pass;

typedef daeSmartRef<domCg_pass> domCg_passRef;
typedef daeTArray<domCg_passRef> domCg_pass_Array;

class domProfile_cg;

typedef daeSmartRef<domProfile_cg> domProfile_cgRef;
typedef daeTArray<domProfile_cgRef> domProfile_cg_Array;

class domGles_texture_constant;

typedef daeSmartRef<domGles_texture_constant> domGles_texture_constantRef;
typedef daeTArray<domGles_texture_constantRef> domGles_texture_constant_Array;

class domGles_texenv_command;

typedef daeSmartRef<domGles_texenv_command> domGles_texenv_commandRef;
typedef daeTArray<domGles_texenv_commandRef> domGles_texenv_command_Array;

class domGles_texcombiner_argument_rgb;

typedef daeSmartRef<domGles_texcombiner_argument_rgb> domGles_texcombiner_argument_rgbRef;
typedef daeTArray<domGles_texcombiner_argument_rgbRef> domGles_texcombiner_argument_rgb_Array;

class domGles_texcombiner_argument_alpha;

typedef daeSmartRef<domGles_texcombiner_argument_alpha> domGles_texcombiner_argument_alphaRef;
typedef daeTArray<domGles_texcombiner_argument_alphaRef> domGles_texcombiner_argument_alpha_Array;

class domGles_texcombiner_command_rgb;

typedef daeSmartRef<domGles_texcombiner_command_rgb> domGles_texcombiner_command_rgbRef;
typedef daeTArray<domGles_texcombiner_command_rgbRef> domGles_texcombiner_command_rgb_Array;

class domGles_texcombiner_command_alpha;

typedef daeSmartRef<domGles_texcombiner_command_alpha> domGles_texcombiner_command_alphaRef;
typedef daeTArray<domGles_texcombiner_command_alphaRef> domGles_texcombiner_command_alpha_Array;

class domGles_texcombiner_command;

typedef daeSmartRef<domGles_texcombiner_command> domGles_texcombiner_commandRef;
typedef daeTArray<domGles_texcombiner_commandRef> domGles_texcombiner_command_Array;

class domGles_texture_pipeline;

typedef daeSmartRef<domGles_texture_pipeline> domGles_texture_pipelineRef;
typedef daeTArray<domGles_texture_pipelineRef> domGles_texture_pipeline_Array;

class domGles_sampler;

typedef daeSmartRef<domGles_sampler> domGles_samplerRef;
typedef daeTArray<domGles_samplerRef> domGles_sampler_Array;

class domGles_newparam;

typedef daeSmartRef<domGles_newparam> domGles_newparamRef;
typedef daeTArray<domGles_newparamRef> domGles_newparam_Array;

class domProfile_gles;

typedef daeSmartRef<domProfile_gles> domProfile_glesRef;
typedef daeTArray<domProfile_glesRef> domProfile_gles_Array;

class domEffect;

typedef daeSmartRef<domEffect> domEffectRef;
typedef daeTArray<domEffectRef> domEffect_Array;

class domLibrary_effects;

typedef daeSmartRef<domLibrary_effects> domLibrary_effectsRef;
typedef daeTArray<domLibrary_effectsRef> domLibrary_effects_Array;

class domBox;

typedef daeSmartRef<domBox> domBoxRef;
typedef daeTArray<domBoxRef> domBox_Array;

class domPlane;

typedef daeSmartRef<domPlane> domPlaneRef;
typedef daeTArray<domPlaneRef> domPlane_Array;

class domSphere;

typedef daeSmartRef<domSphere> domSphereRef;
typedef daeTArray<domSphereRef> domSphere_Array;

class domEllipsoid;

typedef daeSmartRef<domEllipsoid> domEllipsoidRef;
typedef daeTArray<domEllipsoidRef> domEllipsoid_Array;

class domCylinder;

typedef daeSmartRef<domCylinder> domCylinderRef;
typedef daeTArray<domCylinderRef> domCylinder_Array;

class domCapsule;

typedef daeSmartRef<domCapsule> domCapsuleRef;
typedef daeTArray<domCapsuleRef> domCapsule_Array;

class domForce_field;

typedef daeSmartRef<domForce_field> domForce_fieldRef;
typedef daeTArray<domForce_fieldRef> domForce_field_Array;

class domLibrary_force_fields;

typedef daeSmartRef<domLibrary_force_fields> domLibrary_force_fieldsRef;
typedef daeTArray<domLibrary_force_fieldsRef> domLibrary_force_fields_Array;

class domPhysics_material;

typedef daeSmartRef<domPhysics_material> domPhysics_materialRef;
typedef daeTArray<domPhysics_materialRef> domPhysics_material_Array;

class domLibrary_physics_materials;

typedef daeSmartRef<domLibrary_physics_materials> domLibrary_physics_materialsRef;
typedef daeTArray<domLibrary_physics_materialsRef> domLibrary_physics_materials_Array;

class domEdges;

typedef daeSmartRef<domEdges> domEdgesRef;
typedef daeTArray<domEdgesRef> domEdges_Array;

class domWires;

typedef daeSmartRef<domWires> domWiresRef;
typedef daeTArray<domWiresRef> domWires_Array;

class domFaces;

typedef daeSmartRef<domFaces> domFacesRef;
typedef daeTArray<domFacesRef> domFaces_Array;

class domShells;

typedef daeSmartRef<domShells> domShellsRef;
typedef daeTArray<domShellsRef> domShells_Array;

class domSolids;

typedef daeSmartRef<domSolids> domSolidsRef;
typedef daeTArray<domSolidsRef> domSolids_Array;

class domLine;

typedef daeSmartRef<domLine> domLineRef;
typedef daeTArray<domLineRef> domLine_Array;

class domCircle;

typedef daeSmartRef<domCircle> domCircleRef;
typedef daeTArray<domCircleRef> domCircle_Array;

class domEllipse;

typedef daeSmartRef<domEllipse> domEllipseRef;
typedef daeTArray<domEllipseRef> domEllipse_Array;

class domParabola;

typedef daeSmartRef<domParabola> domParabolaRef;
typedef daeTArray<domParabolaRef> domParabola_Array;

class domHyperbola;

typedef daeSmartRef<domHyperbola> domHyperbolaRef;
typedef daeTArray<domHyperbolaRef> domHyperbola_Array;

class domTorus;

typedef daeSmartRef<domTorus> domTorusRef;
typedef daeTArray<domTorusRef> domTorus_Array;

class domPcurves;

typedef daeSmartRef<domPcurves> domPcurvesRef;
typedef daeTArray<domPcurvesRef> domPcurves_Array;

class domSidref_array;

typedef daeSmartRef<domSidref_array> domSidref_arrayRef;
typedef daeTArray<domSidref_arrayRef> domSidref_array_Array;

class domSource;

typedef daeSmartRef<domSource> domSourceRef;
typedef daeTArray<domSourceRef> domSource_Array;

class domAnimation;

typedef daeSmartRef<domAnimation> domAnimationRef;
typedef daeTArray<domAnimationRef> domAnimation_Array;

class domLibrary_animations;

typedef daeSmartRef<domLibrary_animations> domLibrary_animationsRef;
typedef daeTArray<domLibrary_animationsRef> domLibrary_animations_Array;

class domSkin;

typedef daeSmartRef<domSkin> domSkinRef;
typedef daeTArray<domSkinRef> domSkin_Array;

class domConvex_mesh;

typedef daeSmartRef<domConvex_mesh> domConvex_meshRef;
typedef daeTArray<domConvex_meshRef> domConvex_mesh_Array;

class domMesh;

typedef daeSmartRef<domMesh> domMeshRef;
typedef daeTArray<domMeshRef> domMesh_Array;

class domSpline;

typedef daeSmartRef<domSpline> domSplineRef;
typedef daeTArray<domSplineRef> domSpline_Array;

class domNurbs_surface;

typedef daeSmartRef<domNurbs_surface> domNurbs_surfaceRef;
typedef daeTArray<domNurbs_surfaceRef> domNurbs_surface_Array;

class domNurbs;

typedef daeSmartRef<domNurbs> domNurbsRef;
typedef daeTArray<domNurbsRef> domNurbs_Array;

class domMorph;

typedef daeSmartRef<domMorph> domMorphRef;
typedef daeTArray<domMorphRef> domMorph_Array;

class domController;

typedef daeSmartRef<domController> domControllerRef;
typedef daeTArray<domControllerRef> domController_Array;

class domLibrary_controllers;

typedef daeSmartRef<domLibrary_controllers> domLibrary_controllersRef;
typedef daeTArray<domLibrary_controllersRef> domLibrary_controllers_Array;

class domOrigin;

typedef daeSmartRef<domOrigin> domOriginRef;
typedef daeTArray<domOriginRef> domOrigin_Array;

class domOrient;

typedef daeSmartRef<domOrient> domOrientRef;
typedef daeTArray<domOrientRef> domOrient_Array;

class domCurve;

typedef daeSmartRef<domCurve> domCurveRef;
typedef daeTArray<domCurveRef> domCurve_Array;

class domSurface_curves;

typedef daeSmartRef<domSurface_curves> domSurface_curvesRef;
typedef daeTArray<domSurface_curvesRef> domSurface_curves_Array;

class domCurves;

typedef daeSmartRef<domCurves> domCurvesRef;
typedef daeTArray<domCurvesRef> domCurves_Array;

class domSwept_surface;

typedef daeSmartRef<domSwept_surface> domSwept_surfaceRef;
typedef daeTArray<domSwept_surfaceRef> domSwept_surface_Array;

class domCone;

typedef daeSmartRef<domCone> domConeRef;
typedef daeTArray<domConeRef> domCone_Array;

class domSurface;

typedef daeSmartRef<domSurface> domSurfaceRef;
typedef daeTArray<domSurfaceRef> domSurface_Array;

class domSurfaces;

typedef daeSmartRef<domSurfaces> domSurfacesRef;
typedef daeTArray<domSurfacesRef> domSurfaces_Array;

class domBrep;

typedef daeSmartRef<domBrep> domBrepRef;
typedef daeTArray<domBrepRef> domBrep_Array;

class domGeometry;

typedef daeSmartRef<domGeometry> domGeometryRef;
typedef daeTArray<domGeometryRef> domGeometry_Array;

class domLibrary_geometries;

typedef daeSmartRef<domLibrary_geometries> domLibrary_geometriesRef;
typedef daeTArray<domLibrary_geometriesRef> domLibrary_geometries_Array;

class domLimits_sub;

typedef daeSmartRef<domLimits_sub> domLimits_subRef;
typedef daeTArray<domLimits_subRef> domLimits_sub_Array;

class domInstance_joint;

typedef daeSmartRef<domInstance_joint> domInstance_jointRef;
typedef daeTArray<domInstance_jointRef> domInstance_joint_Array;

class domTargetable_float4;

typedef daeSmartRef<domTargetable_float4> domTargetable_float4Ref;
typedef daeTArray<domTargetable_float4Ref> domTargetable_float4_Array;

class domRotate;

typedef daeSmartRef<domRotate> domRotateRef;
typedef daeTArray<domRotateRef> domRotate_Array;

class domLink;

typedef daeSmartRef<domLink> domLinkRef;
typedef daeTArray<domLinkRef> domLink_Array;

class domRigid_constraint;

typedef daeSmartRef<domRigid_constraint> domRigid_constraintRef;
typedef daeTArray<domRigid_constraintRef> domRigid_constraint_Array;

class domNode;

typedef daeSmartRef<domNode> domNodeRef;
typedef daeTArray<domNodeRef> domNode_Array;

class domLibrary_nodes;

typedef daeSmartRef<domLibrary_nodes> domLibrary_nodesRef;
typedef daeTArray<domLibrary_nodesRef> domLibrary_nodes_Array;

class domVisual_scene;

typedef daeSmartRef<domVisual_scene> domVisual_sceneRef;
typedef daeTArray<domVisual_sceneRef> domVisual_scene_Array;

class domLibrary_visual_scenes;

typedef daeSmartRef<domLibrary_visual_scenes> domLibrary_visual_scenesRef;
typedef daeTArray<domLibrary_visual_scenesRef> domLibrary_visual_scenes_Array;

class domRigid_body;

typedef daeSmartRef<domRigid_body> domRigid_bodyRef;
typedef daeTArray<domRigid_bodyRef> domRigid_body_Array;

class domInstance_rigid_body;

typedef daeSmartRef<domInstance_rigid_body> domInstance_rigid_bodyRef;
typedef daeTArray<domInstance_rigid_bodyRef> domInstance_rigid_body_Array;

class domInstance_physics_model;

typedef daeSmartRef<domInstance_physics_model> domInstance_physics_modelRef;
typedef daeTArray<domInstance_physics_modelRef> domInstance_physics_model_Array;

class domPhysics_model;

typedef daeSmartRef<domPhysics_model> domPhysics_modelRef;
typedef daeTArray<domPhysics_modelRef> domPhysics_model_Array;

class domLibrary_physics_models;

typedef daeSmartRef<domLibrary_physics_models> domLibrary_physics_modelsRef;
typedef daeTArray<domLibrary_physics_modelsRef> domLibrary_physics_models_Array;

class domPhysics_scene;

typedef daeSmartRef<domPhysics_scene> domPhysics_sceneRef;
typedef daeTArray<domPhysics_sceneRef> domPhysics_scene_Array;

class domLibrary_physics_scenes;

typedef daeSmartRef<domLibrary_physics_scenes> domLibrary_physics_scenesRef;
typedef daeTArray<domLibrary_physics_scenesRef> domLibrary_physics_scenes_Array;

class domKinematics_newparam;

typedef daeSmartRef<domKinematics_newparam> domKinematics_newparamRef;
typedef daeTArray<domKinematics_newparamRef> domKinematics_newparam_Array;

class domKinematics_frame;

typedef daeSmartRef<domKinematics_frame> domKinematics_frameRef;
typedef daeTArray<domKinematics_frameRef> domKinematics_frame_Array;

class domCommon_param;

typedef daeSmartRef<domCommon_param> domCommon_paramRef;
typedef daeTArray<domCommon_paramRef> domCommon_param_Array;

class domCommon_int_or_param;

typedef daeSmartRef<domCommon_int_or_param> domCommon_int_or_paramRef;
typedef daeTArray<domCommon_int_or_paramRef> domCommon_int_or_param_Array;

class domCommon_bool_or_param;

typedef daeSmartRef<domCommon_bool_or_param> domCommon_bool_or_paramRef;
typedef daeTArray<domCommon_bool_or_paramRef> domCommon_bool_or_param_Array;

class domCommon_float_or_param;

typedef daeSmartRef<domCommon_float_or_param> domCommon_float_or_paramRef;
typedef daeTArray<domCommon_float_or_paramRef> domCommon_float_or_param_Array;

class domKinematics_limits;

typedef daeSmartRef<domKinematics_limits> domKinematics_limitsRef;
typedef daeTArray<domKinematics_limitsRef> domKinematics_limits_Array;

class domCommon_sidref_or_param;

typedef daeSmartRef<domCommon_sidref_or_param> domCommon_sidref_or_paramRef;
typedef daeTArray<domCommon_sidref_or_paramRef> domCommon_sidref_or_param_Array;

class domBind_joint_axis;

typedef daeSmartRef<domBind_joint_axis> domBind_joint_axisRef;
typedef daeTArray<domBind_joint_axisRef> domBind_joint_axis_Array;

class domBind_kinematics_model;

typedef daeSmartRef<domBind_kinematics_model> domBind_kinematics_modelRef;
typedef daeTArray<domBind_kinematics_modelRef> domBind_kinematics_model_Array;

class domKinematics_connect_param;

typedef daeSmartRef<domKinematics_connect_param> domKinematics_connect_paramRef;
typedef daeTArray<domKinematics_connect_paramRef> domKinematics_connect_param_Array;

class domKinematics_setparam;

typedef daeSmartRef<domKinematics_setparam> domKinematics_setparamRef;
typedef daeTArray<domKinematics_setparamRef> domKinematics_setparam_Array;

class domInstance_kinematics_scene;

typedef daeSmartRef<domInstance_kinematics_scene> domInstance_kinematics_sceneRef;
typedef daeTArray<domInstance_kinematics_sceneRef> domInstance_kinematics_scene_Array;

class domKinematics_index;

typedef daeSmartRef<domKinematics_index> domKinematics_indexRef;
typedef daeTArray<domKinematics_indexRef> domKinematics_index_Array;

class domKinematics_param;

typedef daeSmartRef<domKinematics_param> domKinematics_paramRef;
typedef daeTArray<domKinematics_paramRef> domKinematics_param_Array;

class domKinematics_bind;

typedef daeSmartRef<domKinematics_bind> domKinematics_bindRef;
typedef daeTArray<domKinematics_bindRef> domKinematics_bind_Array;

class domMotion_axis_info;

typedef daeSmartRef<domMotion_axis_info> domMotion_axis_infoRef;
typedef daeTArray<domMotion_axis_infoRef> domMotion_axis_info_Array;

class domInstance_kinematics_model;

typedef daeSmartRef<domInstance_kinematics_model> domInstance_kinematics_modelRef;
typedef daeTArray<domInstance_kinematics_modelRef> domInstance_kinematics_model_Array;

class domInstance_articulated_system;

typedef daeSmartRef<domInstance_articulated_system> domInstance_articulated_systemRef;
typedef daeTArray<domInstance_articulated_systemRef> domInstance_articulated_system_Array;

class domKinematics_scene;

typedef daeSmartRef<domKinematics_scene> domKinematics_sceneRef;
typedef daeTArray<domKinematics_sceneRef> domKinematics_scene_Array;

class domLibrary_kinematics_scenes;

typedef daeSmartRef<domLibrary_kinematics_scenes> domLibrary_kinematics_scenesRef;
typedef daeTArray<domLibrary_kinematics_scenesRef> domLibrary_kinematics_scenes_Array;

class domCommon_float2_or_param;

typedef daeSmartRef<domCommon_float2_or_param> domCommon_float2_or_paramRef;
typedef daeTArray<domCommon_float2_or_paramRef> domCommon_float2_or_param_Array;

class domMotion_effector_info;

typedef daeSmartRef<domMotion_effector_info> domMotion_effector_infoRef;
typedef daeTArray<domMotion_effector_infoRef> domMotion_effector_info_Array;

class domMotion_technique;

typedef daeSmartRef<domMotion_technique> domMotion_techniqueRef;
typedef daeTArray<domMotion_techniqueRef> domMotion_technique_Array;

class domMotion;

typedef daeSmartRef<domMotion> domMotionRef;
typedef daeTArray<domMotionRef> domMotion_Array;

class domMinmax;

typedef daeSmartRef<domMinmax> domMinmaxRef;
typedef daeTArray<domMinmaxRef> domMinmax_Array;

class domJoint_limits;

typedef daeSmartRef<domJoint_limits> domJoint_limitsRef;
typedef daeTArray<domJoint_limitsRef> domJoint_limits_Array;

class domAxis;

typedef daeSmartRef<domAxis> domAxisRef;
typedef daeTArray<domAxisRef> domAxis_Array;

class domAxis_constraint;

typedef daeSmartRef<domAxis_constraint> domAxis_constraintRef;
typedef daeTArray<domAxis_constraintRef> domAxis_constraint_Array;

class domJoint;

typedef daeSmartRef<domJoint> domJointRef;
typedef daeTArray<domJointRef> domJoint_Array;

class domLibrary_joints;

typedef daeSmartRef<domLibrary_joints> domLibrary_jointsRef;
typedef daeTArray<domLibrary_jointsRef> domLibrary_joints_Array;

class domFormula_technique;

typedef daeSmartRef<domFormula_technique> domFormula_techniqueRef;
typedef daeTArray<domFormula_techniqueRef> domFormula_technique_Array;

class domFormula_newparam;

typedef daeSmartRef<domFormula_newparam> domFormula_newparamRef;
typedef daeTArray<domFormula_newparamRef> domFormula_newparam_Array;

class domFormula_setparam;

typedef daeSmartRef<domFormula_setparam> domFormula_setparamRef;
typedef daeTArray<domFormula_setparamRef> domFormula_setparam_Array;

class domInstance_formula;

typedef daeSmartRef<domInstance_formula> domInstance_formulaRef;
typedef daeTArray<domInstance_formulaRef> domInstance_formula_Array;

class domAnimation_clip;

typedef daeSmartRef<domAnimation_clip> domAnimation_clipRef;
typedef daeTArray<domAnimation_clipRef> domAnimation_clip_Array;

class domLibrary_animation_clips;

typedef daeSmartRef<domLibrary_animation_clips> domLibrary_animation_clipsRef;
typedef daeTArray<domLibrary_animation_clipsRef> domLibrary_animation_clips_Array;

class domFormula;

typedef daeSmartRef<domFormula> domFormulaRef;
typedef daeTArray<domFormulaRef> domFormula_Array;

class domKinematics_model_technique;

typedef daeSmartRef<domKinematics_model_technique> domKinematics_model_techniqueRef;
typedef daeTArray<domKinematics_model_techniqueRef> domKinematics_model_technique_Array;

class domKinematics_model;

typedef daeSmartRef<domKinematics_model> domKinematics_modelRef;
typedef daeTArray<domKinematics_modelRef> domKinematics_model_Array;

class domLibrary_kinematics_models;

typedef daeSmartRef<domLibrary_kinematics_models> domLibrary_kinematics_modelsRef;
typedef daeTArray<domLibrary_kinematics_modelsRef> domLibrary_kinematics_models_Array;

class domKinematics_axis_info;

typedef daeSmartRef<domKinematics_axis_info> domKinematics_axis_infoRef;
typedef daeTArray<domKinematics_axis_infoRef> domKinematics_axis_info_Array;

class domKinematics_technique;

typedef daeSmartRef<domKinematics_technique> domKinematics_techniqueRef;
typedef daeTArray<domKinematics_techniqueRef> domKinematics_technique_Array;

class domKinematics;

typedef daeSmartRef<domKinematics> domKinematicsRef;
typedef daeTArray<domKinematicsRef> domKinematics_Array;

class domArticulated_system;

typedef daeSmartRef<domArticulated_system> domArticulated_systemRef;
typedef daeTArray<domArticulated_systemRef> domArticulated_system_Array;

class domLibrary_articulated_systems;

typedef daeSmartRef<domLibrary_articulated_systems> domLibrary_articulated_systemsRef;
typedef daeTArray<domLibrary_articulated_systemsRef> domLibrary_articulated_systems_Array;

class domLibrary_formulas;

typedef daeSmartRef<domLibrary_formulas> domLibrary_formulasRef;
typedef daeTArray<domLibrary_formulasRef> domLibrary_formulas_Array;

class domFx_sampler_states;

typedef daeSmartRef<domFx_sampler_states> domFx_sampler_statesRef;
typedef daeTArray<domFx_sampler_statesRef> domFx_sampler_states_Array;

class domFx_setparam;

typedef daeSmartRef<domFx_setparam> domFx_setparamRef;
typedef daeTArray<domFx_setparamRef> domFx_setparam_Array;

class domGl_pipeline_settings;

typedef daeSmartRef<domGl_pipeline_settings> domGl_pipeline_settingsRef;
typedef daeTArray<domGl_pipeline_settingsRef> domGl_pipeline_settings_Array;

class domGles2_pipeline_settings;

typedef daeSmartRef<domGles2_pipeline_settings> domGles2_pipeline_settingsRef;
typedef daeTArray<domGles2_pipeline_settingsRef> domGles2_pipeline_settings_Array;

class domGles2_value;

typedef daeSmartRef<domGles2_value> domGles2_valueRef;
typedef daeTArray<domGles2_valueRef> domGles2_value_Array;

class domGlsl_value;

typedef daeSmartRef<domGlsl_value> domGlsl_valueRef;
typedef daeTArray<domGlsl_valueRef> domGlsl_value_Array;

class domCg_param;

typedef daeSmartRef<domCg_param> domCg_paramRef;
typedef daeTArray<domCg_paramRef> domCg_param_Array;

class domGles_sampler_states;

typedef daeSmartRef<domGles_sampler_states> domGles_sampler_statesRef;
typedef daeTArray<domGles_sampler_statesRef> domGles_sampler_states_Array;

class domGles_pipeline_settings;

typedef daeSmartRef<domGles_pipeline_settings> domGles_pipeline_settingsRef;
typedef daeTArray<domGles_pipeline_settingsRef> domGles_pipeline_settings_Array;

class domGles_param;

typedef daeSmartRef<domGles_param> domGles_paramRef;
typedef daeTArray<domGles_paramRef> domGles_param_Array;

class domFx_profile;

typedef daeSmartRef<domFx_profile> domFx_profileRef;
typedef daeTArray<domFx_profileRef> domFx_profile_Array;

class domCOLLADA;

typedef daeSmartRef<domCOLLADA> domCOLLADARef;
typedef daeTArray<domCOLLADARef> domCOLLADA_Array;


#endif //__DOM_ELEMENTS_H__

