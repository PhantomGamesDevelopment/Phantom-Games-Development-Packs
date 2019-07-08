
singleton Material(CTF_Flag_FlagMat_Red)
{
   mapTo = "FlagMat_Red";
   diffuseColor[0] = "0.64 0.64 0.64 1";
   specular[0] = "0.5 0.5 0.5 1";
   specularPower[0] = "50";
   doubleSided = "1";
   translucentBlendOp = "None";
   diffuseMap[0] = "art/shapes/CTF/Flag_Diffuse_Red.dds";
   normalMap[0] = "art/shapes/CTF/Flag_Normal.png";
   specularMap[0] = "art/shapes/CTF/Flag_Specular.png";
};

singleton Material(CTF_Flag_FlagMat_Blue : CTF_Flag_FlagMat_Red)
{
   mapTo = "FlagMat_Blue";
   diffuseMap[0] = "art/shapes/CTF/Flag_Diffuse_Blue.dds";
   normalMap[0] = "art/shapes/CTF/Flag_Normal.png";
   specularMap[0] = "art/shapes/CTF/Flag_Specular.png";
};
