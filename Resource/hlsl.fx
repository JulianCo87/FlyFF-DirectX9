
float4x4	mWVP;           // ����
float4x4	mProj;
float4x4	mView;

float4      vLightDir;      // ��������

float3      vEyePos;		// ī�޶���ġ
float4      vFog;		// (Far/(Far-Near), -1/(Far-Near))


float4x3	mBoneMatrix[28];

float4 GetAmbientDiffuse( float3 N, float3 L, const float basicAmbient, const float minAmbient );
float4 GetSpecular( float3 R, float3 L );

texture	Tex;            // �ؽ�ó
texture Tex_Diffuse;
texture Tex_Bump;
texture Tex_Specular;
texture	Tex_EnvironmentMap;

// **********************************************
// �ؽ�ó ������Ʈ ����
// **********************************************
sampler Samp_Diffuse = sampler_state
{
    Texture = <Tex_Diffuse>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler Samp_Bump = sampler_state
{
    Texture = <Tex_Bump>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

sampler Samp_Specular = sampler_state
{
    Texture = <Tex_Specular>;
    MinFilter = NONE;
    MagFilter = NONE;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

samplerCUBE EnvironmentSampler = sampler_state
{  
    Texture = (Tex_EnvironmentMap); 
    MipFilter = LINEAR; 
    MinFilter = LINEAR; 
    MagFilter = LINEAR; 
}; 



///////////////////////////////////////////////////////////////////////////////////////////////////

// light intensity
float4 I_a = { 0.6f, 0.6f, 0.6f, 1.0f };    // ambient
float4 I_d = { 0.4f, 0.4f, 0.4f, 1.0f };    // diffuse
float4 I_s = { 0.7f, 0.7f, 0.7f, 1.0f };    // specular

float4 k_a : MATERIALAMBIENT = { 0.65f, 0.65f, 0.65f, 1.0f };    // ambient
float4 k_d : MATERIALDIFFUSE = { 0.55f, 0.55f, 0.55f, 1.0f };    // diffuse
float4 k_s : MATERIALSPECULAR= { 0.3f, 0.3f, 0.3f, 1.0f };    // specular
float  n   : MATERIALPOWER = 64.0f;                           // power


// **********************************************
// �������̴����� �ȼ����̴��� �ѱ�� ������
// **********************************************
struct VS_OUTPUT_Phong
{
	float4 Pos			: POSITION;
	float2 Tex			: TEXCOORD0;
	float4 Diffuse		: COLOR0;
	float4 Specular		: COLOR1;
	float3 N			: TEXCOORD1;
	float3 Eye			: TEXCOORD2;
	float  Fog			: FOG;
};

struct VS_OUTPUT_Gouraud
{
	float4 Pos			: POSITION;
	float2 Tex			: TEXCOORD0;
	float4 Diffuse		: COLOR0;
	float  Fog			: FOG;
};


// **********************************************
// ��� ���̵�
// ----------------------------------------------
// �������̴�
VS_OUTPUT_Gouraud VS_Gouraud( float4 Pos : POSITION, float2 Tex : TEXCOORD0, float3 Normal : NORMAL )
{
	VS_OUTPUT_Gouraud Out = (VS_OUTPUT_Gouraud)0;     // ��µ���Ÿ �ʱ�ȭ

	Out.Pos = mul( Pos, mWVP );       // ���� ���庯ȯ

	float3 L = -vLightDir;
	float3 N = normalize( Normal );

	float3	V = normalize(vEyePos - Pos.xyz);   // �ü�����
	float3  H = normalize( L+V );				// HALF vector(L+V)

	Out.Diffuse   = I_a * k_a + I_d * k_d * max(0, dot(N, L));

	Out.Tex = Tex;

	// �Ÿ�����
	Out.Fog = vFog.x + Out.Pos.w * vFog.y;

	return Out;  // �ȼ����̴��� �ѱ��
}

// �ȼ����̴�
float4 PS_Gouraud( VS_OUTPUT_Gouraud In ) : COLOR
{
	return In.Diffuse * tex2D( Samp_Diffuse, In.Tex );// + In.Specular;
}	       


// -------------------------------------------------------------
// �������̴����� �ȼ����̴��� �ѱ�� ������
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos			: POSITION;

    float2 Tex			: TEXCOORD0;	// �����ؽ�ó ��ǥ
    float3 L			: TEXCOORD1;	// ��������
    float3 E			: TEXCOORD2;	// ��������
	float  Fog			: FOG;
};
// -------------------------------------------------------------
// ��鷻��
// -------------------------------------------------------------
VS_OUTPUT VS_BUMP(
      float4 Pos      : POSITION,          // ������ġ��ǥ
      float3 Normal   : NORMAL,            // ��������
      float3 Tangent  : TANGENT0,          // ��������
      float2 Texcoord : TEXCOORD0          // �ؽ�ó��ǥ
){
	VS_OUTPUT Out = (VS_OUTPUT)0;        // ��µ�����
	
	// ��ǥ��ȯ
	Out.Pos = mul(Pos, mWVP);
	
	// ���ÿ� �ؽ�ó��ǥ
	Out.Tex = Texcoord;

	// ��ǥ�躯ȯ ����
	float3 N = Normal;
	float3 T = Tangent;
	float3 B = cross(N,T);

	// �ݿ��ݻ�� ����
	float3 E = vEyePos - Pos.xyz;	// �ü�����
	Out.E.x = dot(E,T);
	Out.E.y = dot(E,B);
	Out.E.z = dot(E,N);

	float3 L = -vLightDir.xyz;		// ��������
	Out.L.x = dot(L,T);
	Out.L.y = dot(L,B);
	Out.L.z = dot(L,N);

	// �Ÿ�����
	Out.Fog = vFog.x + Out.Pos.w * vFog.y;

	return Out;
}
// -------------------------------------------------------------
float4 PS_BUMP(VS_OUTPUT In) : COLOR
{   
	float3 N = 2.0f*tex2D( Samp_Bump, In.Tex ).xyz-1.0f;// ���������κ��� ����
	float3 L = normalize(In.L);						// ��������

	float4 fAmbientDiffuse;
	float4 fDiffuseTexture;
	float4 fSpecTexture;

	fAmbientDiffuse = GetAmbientDiffuse( N, L, 0.0f, 0.0f );
	fDiffuseTexture = tex2D( Samp_Diffuse, In.Tex );
	fSpecTexture    = tex2D( Samp_Specular, In.Tex );

	if( fSpecTexture.r == 0.0f && fSpecTexture.g == 0.0f && fSpecTexture.b == 0.0f )
	{
		return fAmbientDiffuse * fDiffuseTexture;
	}
	else
	{
		float4 fColor = fAmbientDiffuse * fDiffuseTexture;
		fColor  *= fSpecTexture;

		// ����ŧ�� �ֱ� ���������� �˻� �������̸� �ݼ����� �ȳ���
		if( fDiffuseTexture.r > 0.0f && fDiffuseTexture.g > 0.0f && fDiffuseTexture.b > 0.0f )
		{
			float3 R = reflect(-normalize(In.E), N);
			fColor  += GetSpecular( R, L );
		}

		return fColor;
	}
}

float4 PS_Bump_Env(VS_OUTPUT In) : COLOR
{   
	float3 N = 2.0f*tex2D( Samp_Bump, In.Tex ).xyz-1.0f;// ���������κ��� ����
	float3 L = normalize(In.L);						// ��������

	float4 fAmbientDiffuse;
	float4 fDiffuseTexture;
	float4 fSpecTexture;

	fAmbientDiffuse = GetAmbientDiffuse( N, L, 0.0f, 0.0f );
	fDiffuseTexture = tex2D( Samp_Diffuse, In.Tex );
	fSpecTexture    = tex2D( Samp_Specular, In.Tex );


   	float3 R = reflect(-normalize(In.E), N);
  	float4 fEnvTexture = texCUBE(EnvironmentSampler, R);
	float4 flerpColor = lerp( fDiffuseTexture, fEnvTexture, 0.2f );

	float4 fColor = fAmbientDiffuse * flerpColor;
	//fColor = fColor * flerpColor;


	if( fSpecTexture.r == 0.0f && fSpecTexture.g == 0.0f && fSpecTexture.b == 0.0f )
	{
		return fAmbientDiffuse * fDiffuseTexture;
	}
	else
	{
		fColor  *= fSpecTexture;

		// ����ŧ�� �ֱ� ���������� �˻� �������̸� �ݼ����� �ȳ���
		if( fDiffuseTexture.r > 0.0f && fDiffuseTexture.g > 0.0f && fDiffuseTexture.b > 0.0f )
		{
			fColor  += GetSpecular( R, L );
		}

		return fColor;
	}
}
float4 PS_BUMP_ALPHA(VS_OUTPUT In) : COLOR
{   
	float3 N = 2.0f*tex2D( Samp_Bump, In.Tex ).xyz-1.0f;// ���������κ��� ����
	float3 L = normalize(In.L);						// ��������

	float4 fAmbientDiffuse;

	fAmbientDiffuse = GetAmbientDiffuse( N, L, 0.0f, 0.0f );

	float4 fDiffuseTexture;
	fDiffuseTexture = tex2D( Samp_Diffuse, In.Tex );

	float4 fColor;

	fColor = (fAmbientDiffuse * fDiffuseTexture );
	return fColor;
}


struct VS_OUTPUT_SPEC
{
	float4 Pos			: POSITION;
	float2 Tex			: TEXCOORD0;
	float3 N			: TEXCOORD1;
	float3 Eye			: TEXCOORD2;
	float  Fog			: FOG;
};

VS_OUTPUT_SPEC VS_SPEC( float4 Pos : POSITION, float2 Tex : TEXCOORD0, float3 Normal : NORMAL )
{
	VS_OUTPUT_SPEC Out = (VS_OUTPUT_SPEC)0;     // ��µ���Ÿ �ʱ�ȭ

	Out.Pos = mul( Pos, mWVP );       // ���� ���庯ȯ

	float3 L   = -vLightDir;
	Out.N      = normalize( Normal ); // ���� �������
	Out.Eye    = vEyePos - Pos.xyz;   // �ü�����

	Out.Tex = Tex;                    // �ؽ�ó ����

	// �Ÿ�����
	Out.Fog = vFog.x + Out.Pos.w * vFog.y;

	return Out;  // �ȼ����̴��� �ѱ��
}

float4 PS_SPEC( VS_OUTPUT_SPEC In ) : COLOR
{
	// �� �� �ݿ� �ݻ�
	float3	L = normalize(-vLightDir.xyz);
	float3	V = normalize( In.Eye );

	// HALF vector(L+V)
	float3  H = normalize( L+V ); 
	float3  N = normalize( In.N );

	float4 fAmbientDiffuse;
	float4 fDiffuseTexture;
	float4 fSpecTexture;

	fAmbientDiffuse = I_a * k_a + I_d * k_d * max(0, dot(N, L));
	fDiffuseTexture = tex2D( Samp_Diffuse, In.Tex );
	fSpecTexture    = tex2D( Samp_Specular, In.Tex );

   	float3 R = reflect(-normalize(In.Eye), N);
  	float4 fEnvTexture = texCUBE(EnvironmentSampler, R);
	float4 flerpColor = lerp( fDiffuseTexture, fEnvTexture, 0.25f );

	float4 fColor = fAmbientDiffuse * flerpColor;


	if( fSpecTexture.r == 0.0f && fSpecTexture.g == 0.0f && fSpecTexture.b == 0.0f )
	{
		return ( fAmbientDiffuse * fDiffuseTexture );
	}
	else
	{
		fColor  *= fSpecTexture;

		// ����ŧ�� �ֱ� ���������� �˻� �������̸� �ݼ����� �ȳ���
		if( fDiffuseTexture.r > 0.0f && fDiffuseTexture.g > 0.0f && fDiffuseTexture.b > 0.0f )
		{
			fColor  += ( GetSpecular( R, L ) * fSpecTexture );
		}

		return fColor;
	}
}

float4 PS_SPEC_ALPHA( VS_OUTPUT_SPEC In ) : COLOR
{
	// �� �� �ݿ� �ݻ�
	float3	L = normalize(-vLightDir.xyz);
	float3	V = normalize( In.Eye );

	// HALF vector(L+V)
	float3  H = normalize( L+V ); 
	float3  N = normalize( In.N );

	float4 fAmbientDiffuse;
	float4 fDiffuseTexture;

	fAmbientDiffuse = I_a * k_a + I_d * k_d * max(0, dot(N, L));
	fDiffuseTexture = tex2D( Samp_Diffuse, In.Tex );

	float4 fColor = fAmbientDiffuse * fDiffuseTexture;
	return fColor;
}

// ������� ����
struct VS_OUTPUT_SKIN
{
	float4 Pos     : POSITION;
	float4 Diffuse : COLOR0;
	float2 Tex     : TEXCOORD0;
	float  Fog	   : FOG;
};

// �������̴� �Լ� ����
VS_OUTPUT_SKIN VS_SkinBasic( 
				    float3 Pos			  : POSITION,
					float4 BlendWeights   : BLENDWEIGHT,
					float4 BlendIndices	  : BLENDINDICES,
					float3 Normal		  : NORMAL,
					float2 Tex			  : TEXCOORD0 )
{
	// �������
    VS_OUTPUT_SKIN Out = (VS_OUTPUT_SKIN)0;

	// �ӽú���
	float3 p;
	float3 p2 = float3( 0, 0, 0 );

	// ù��° ��Ʈ���� �ȷ�Ʈ����
	p = mul( float4(Pos,1), mBoneMatrix[BlendIndices.x/3] );
	p = p * BlendWeights.x;							
	p2 = float4(p,0);						

	// �ι�° ��Ʈ���� �ȷ�Ʈ����
	p = mul( float4(Pos,1), mBoneMatrix[BlendIndices.y/3] );
	p = p * BlendWeights.y;
	p2 = p2 + float4(p,0);

    Out.Pos = mul( float4(p2, 1), mWVP );	
			
	Out.Diffuse   = (I_a * k_a + I_d * k_d * max(0, dot(normalize( Normal ), -vLightDir)));

	Out.Tex = Tex;

	// �Ÿ�����
	Out.Fog = vFog.x + Out.Pos.w * vFog.y;

    return Out;
}


// ������� ����
struct VS_OUTPUT_SKIN_BUMP
{
	float4 Pos     : POSITION;

	float2 Tex     : TEXCOORD0;
    float3 L	   : TEXCOORD1;	// ��������
    float3 E	   : TEXCOORD2;	// ��������
	float  Fog	   : FOG;
};

// �������̴� �Լ� ����
VS_OUTPUT_SKIN_BUMP VS_SkinBUMP(     
	float3 Pos			  : POSITION,
    float4 BlendWeights   : BLENDWEIGHT,
    float4 BlendIndices	  : BLENDINDICES,
    float3 Normal		  : NORMAL,
    float3 Tangent		  : TANGENT0,
    float2 Tex			  : TEXCOORD0 )
{
	// �������
    VS_OUTPUT_SKIN_BUMP Out = (VS_OUTPUT_SKIN_BUMP)0;

	// �ӽú���
	float3 p;
	float3 p2 = float3( 0, 0, 0 );

	// ù��° ��Ʈ���� �ȷ�Ʈ����
	p = mul( float4(Pos,1), mBoneMatrix[BlendIndices.x/3] );
	p = p * BlendWeights.x;							
	p2 = float4(p,0);						

	// �ι�° ��Ʈ���� �ȷ�Ʈ����
	p = mul( float4(Pos,1), mBoneMatrix[BlendIndices.y/3] );
	p = p * BlendWeights.y;
	p2 = p2 + float4(p,0);

    Out.Pos = mul( float4(p2, 1), mWVP );			

	Out.Tex = Tex;

	// ��ǥ�躯ȯ ����
	float3 N = Normal;
	float3 T = Tangent;
	float3 B = cross(N,T);

	// �ݿ��ݻ�� ����
	float3 E = vEyePos - Pos.xyz;	// �ü�����
	Out.E.x = (dot(E,T));
	Out.E.y = (dot(E,B));
	Out.E.z = (dot(E,N));

	float3 L = -vLightDir.xyz;		// ��������
	Out.L.x = (dot(L,T));
	Out.L.y = (dot(L,B));
	Out.L.z = (dot(L,N));

	// �Ÿ�����
	Out.Fog = vFog.x + Out.Pos.w * vFog.y;

    return Out;
}
float4 PS_SkinBUMP(VS_OUTPUT_SKIN_BUMP In) : COLOR
{ 
	float3 N = 2.0f*tex2D( Samp_Bump, In.Tex ).xyz-1.0f;
	float3 L = normalize(In.L);	
						
	float4 fAmbientDiffuse;
	float4 fDiffuseTexture;
	float4 fSpecTexture;

	fAmbientDiffuse = GetAmbientDiffuse( N, L, 0.0f, 0.5f );
	fDiffuseTexture = tex2D( Samp_Diffuse, In.Tex );
	fSpecTexture    = tex2D( Samp_Specular, In.Tex );

	if( fSpecTexture.r == 0.0f && fSpecTexture.g == 0.0f && fSpecTexture.b == 0.0f )
	{
		return fAmbientDiffuse * fDiffuseTexture;
	}
	else
	{
		float4 fColor = fAmbientDiffuse * fDiffuseTexture;
		fColor  *= fSpecTexture;

		// ����ŧ�� �ֱ� ���������� �˻� �������̸� �ݼ����� �ȳ���
		if( fDiffuseTexture.r > 0.0f && fDiffuseTexture.g > 0.0f && fDiffuseTexture.b > 0.0f )
		{
			float3 R = reflect(-normalize(In.E), N);
			fColor  += GetSpecular( R, L );
			//fColor  *= fSpecTexture;
		}

		return fColor;
	}
}

float4 PS_SkinBumpEnv(VS_OUTPUT_SKIN_BUMP In) : COLOR
{ 
	float3 N = 2.0f*tex2D( Samp_Bump, In.Tex ).xyz-1.0f;
	float3 L = normalize(In.L);	
						
	float4 fAmbientDiffuse;
	float4 fDiffuseTexture;
	float4 fSpecTexture;

  	float3 R = reflect(-normalize(In.E), N);

	fAmbientDiffuse = GetAmbientDiffuse( N, L, 0.0f, 0.5f );
	fDiffuseTexture = tex2D( Samp_Diffuse, In.Tex );
	fSpecTexture    = tex2D( Samp_Specular, In.Tex );

//////////////////////////////////////////////////////////////////////////////////////
   	float4 fEnvTexture = texCUBE(EnvironmentSampler, R);
	float4 flerpColor = lerp( fDiffuseTexture, fEnvTexture, 0.2f );

	float4 fColor = fAmbientDiffuse * flerpColor;
	//fColor = fColor * flerpColor;

	if( fSpecTexture.r == 0.0f && fSpecTexture.g == 0.0f && fSpecTexture.b == 0.0f )
	{
		return fAmbientDiffuse * fDiffuseTexture;
	}
	else
	{
		fColor  *= fSpecTexture;

		// ����ŧ�� �ֱ� ���������� �˻� �������̸� �ݼ����� �ȳ���
		if( fDiffuseTexture.r > 0.0f && fDiffuseTexture.g > 0.0f && fDiffuseTexture.b > 0.0f )
		{
			fColor  += GetSpecular( R, L );
		}

		return fColor;
	}
}
float4 PS_SkinBUMP_ALPHA(VS_OUTPUT_SKIN_BUMP In) : COLOR
{ 
	float3 N = 2.0f*tex2D( Samp_Bump, In.Tex ).xyz-1.0f;
	float3 L = normalize(In.L);	
						
	float4 fAmbientDiffuse;

	fAmbientDiffuse = GetAmbientDiffuse( N, L, 0.0f, 0.5f );

	float4 fDiffuseTexture;
	fDiffuseTexture = tex2D( Samp_Diffuse, In.Tex );

	float4 fColor;

	fColor = (fAmbientDiffuse * fDiffuseTexture );

	return fColor;
}





struct VS_OUTPUT_SKIN_SPEC_ENV
{
	float4 Pos     : POSITION;

	float2 Tex     : TEXCOORD0;
    float3 L	   : TEXCOORD1;	// ��������
    float3 E	   : TEXCOORD2;	// ��������
    float3 N	   : TEXCOORD3;	// ��������
	float  Fog	   : FOG;
};

// �������̴� �Լ� ����
VS_OUTPUT_SKIN_SPEC_ENV VS_SkinSPEC_ENV(     
	float3 Pos			  : POSITION,
    float4 BlendWeights   : BLENDWEIGHT,
    float4 BlendIndices	  : BLENDINDICES,
    float3 Normal		  : NORMAL,
    float3 Tangent		  : TANGENT0,
    float2 Tex			  : TEXCOORD0 )
{
	// �������
    VS_OUTPUT_SKIN_SPEC_ENV Out = (VS_OUTPUT_SKIN_SPEC_ENV)0;

	// �ӽú���
	float3 p;
	float3 p2 = float3( 0, 0, 0 );

	// ù��° ��Ʈ���� �ȷ�Ʈ����
	p = mul( float4(Pos,1), mBoneMatrix[BlendIndices.x/3] );
	p = p * BlendWeights.x;							
	p2 = float4(p,0);						

	// �ι�° ��Ʈ���� �ȷ�Ʈ����
	p = mul( float4(Pos,1), mBoneMatrix[BlendIndices.y/3] );
	p = p * BlendWeights.y;
	p2 = p2 + float4(p,0);

    Out.Pos = mul( float4(p2, 1), mWVP );			

	Out.Tex = Tex;

	// �ݿ��ݻ�� ����
	Out.E = vEyePos - Pos.xyz;	// �ü�����
	Out.L = -vLightDir.xyz;		// ��������

	// �Ÿ�����
	Out.Fog = vFog.x + Out.Pos.w * vFog.y;

	Out.N   = normalize( Normal );

    return Out;
}

float4 PS_SkinSPEC_ENV(VS_OUTPUT_SKIN_SPEC_ENV In) : COLOR
{ 
	// �� �� �ݿ� �ݻ�
	float3	L = normalize( In.L);
	float3	V = normalize( In.E );

	// HALF vector(L+V)
	float3  H = normalize( L+V ); 
	float3  N = normalize( In.N );

	float4 fAmbientDiffuse;
	float4 fDiffuseTexture;
	float4 fSpecTexture;

	fAmbientDiffuse = I_a * k_a + I_d * k_d * max(0, dot(N, L));
	fDiffuseTexture = tex2D( Samp_Diffuse, In.Tex );
	fSpecTexture    = tex2D( Samp_Specular, In.Tex );

   	float3 R = reflect(-normalize(In.E), N);
  	float4 fEnvTexture = texCUBE(EnvironmentSampler, R);
	float4 flerpColor = lerp( fDiffuseTexture, fEnvTexture, 0.2f );

	float4 fColor = fAmbientDiffuse * flerpColor;

	if( fSpecTexture.r == 0.0f && fSpecTexture.g == 0.0f && fSpecTexture.b == 0.0f )
	{
		return (fAmbientDiffuse * fDiffuseTexture);
	}
	else
	{
		fColor  *= fSpecTexture;

		// ����ŧ�� �ֱ� ���������� �˻� �������̸� �ݼ����� �ȳ���
		if( fDiffuseTexture.r > 0.0f && fDiffuseTexture.g > 0.0f && fDiffuseTexture.b > 0.0f )
		{
			fColor  += ( GetSpecular( R, L ) * fSpecTexture ) ;
		}

		return fColor;
	}
}

float4 PS_SkinSPEC_ENV_AHPHA(VS_OUTPUT_SKIN_SPEC_ENV In) : COLOR
{ 
	// �� �� �ݿ� �ݻ�
	float3	L = normalize( In.L);
	float3	V = normalize( In.E );

	// HALF vector(L+V)
	float3  H = normalize( L+V ); 
	float3  N = normalize( In.N );

	float4 fAmbientDiffuse;
	float4 fDiffuseTexture;

	fAmbientDiffuse = I_a * k_a + I_d * k_d * max(0, dot(N, L));
	fDiffuseTexture = tex2D( Samp_Diffuse, In.Tex );

	float4 fColor = fAmbientDiffuse * fDiffuseTexture;

	return fColor;
}

float4 GetAmbientDiffuse( float3 N, float3 L, const float basicAmbient, const float minAmbient )
{
	float4 ambient = I_a * k_a;
	float4 diffuse = I_d * k_d;
	float4 diff = max( minAmbient, saturate( dot( N, L ) )+basicAmbient );
	float shadow = saturate(0.05f * diff);

//	return ambient + shadow * (diffuse * diff);

	float4 fResult = ambient + diffuse * shadow;

	if( fResult.a > 1.0f )
		fResult.a = 1.0f;

	if( fResult.r > 1.0f )
		fResult.r = 1.0f;

	if( fResult.g > 1.0f )
		fResult.g = 1.0f;

	if( fResult.b > 1.0f )
		fResult.b = 1.0f;

	return fResult;
}

float4 GetSpecular( float3 R, float3 L )
{
	return I_s * k_s * pow( max( 0, saturate(dot( R, L )) ), n );
}



float4 GlowColor     = float4(0.5f, 0.2f, 0.2f, 1.0f);
float4 GlowAmbient   = float4(0.2f, 0.2f, 0.0f, 0.0f);
float  GlowThickness = 0.1f;
//float  GlowThickness = 0.03f;


struct VSGLOW_OUTPUT
{
    float4 Position : POSITION;
    float4 Diffuse  : COLOR;
	float2 Tex     : TEXCOORD0;
	float  Fog	   : FOG;
};

VSGLOW_OUTPUT VSGlow(
    float4 Position : POSITION, 
    float3 Normal   : NORMAL,
	float2 Tex	    : TEXCOORD0 )
{
    VSGLOW_OUTPUT Out = (VSGLOW_OUTPUT)0;

    float4 N = normalize(mul(Normal, mWVP));     // normal (view space)
    float4 P = mul(Position, mWVP) + GlowThickness * N;    // displaced position (view space)
    float3 A = float3(0, 0, 1);                                 // glow axis

    float Power;

    Power  = dot(N, A);
    Power *= Power;
    Power -= 1;
    Power *= Power;     // Power = (1 - (N.A)^2)^2 [ = ((N.A)^2 - 1)^2 ]

    Out.Position = P;//mul(float4(P, 1), mProj);   // projected position
    Out.Diffuse  = GlowColor * Power + GlowAmbient; // modulated glow color + glow ambient

   	Out.Fog = vFog.x + Out.Position.w * vFog.y;

	Out.Tex = Tex;

    return Out;    
}

float4 PSGlow(VSGLOW_OUTPUT In) : COLOR
{ 
	float4 fDiffuseTexture;
	fDiffuseTexture = tex2D( Samp_Diffuse, In.Tex );

	float4 fColor = In.Diffuse;

	if( fDiffuseTexture.r > 0.0f && fDiffuseTexture.g > 0.0f && fDiffuseTexture.b > 0.0f )
	{
		fColor = 0;
	}

	return fColor;

	/*
	if( fDiffuseTexture.a == 0.0f )
	{
		return 0;
		return In.Diffuse;
	}
	else
	{
		return In.Diffuse;
	}
	*/
}


// **********************************************
// ��ũ��
// **********************************************
technique	TShader
{
	pass p0
	{
		VertexShader = compile vs_1_1 VS_Gouraud();
		PixelShader  = NULL;

		FogEnable = true;			// ���׻��
        FogVertexMode = Linear;		// ���� ����

	    AddressU[0] = Wrap;
	    AddressV[0] = Wrap;
	}

	pass p1
	{
		VertexShader = compile vs_1_1 VS_SPEC();
		PixelShader  = compile ps_2_0 PS_SPEC();

		FogEnable = true;			// ���׻��
        FogVertexMode = Linear;		// ���� ����
		
	    AddressU[0] = Wrap;
	    AddressV[0] = Wrap;
	}
	pass p2
	{
		VertexShader = compile vs_1_1 VS_SPEC();
		PixelShader  = compile ps_2_0 PS_SPEC_ALPHA();

		FogEnable = true;			// ���׻��
        FogVertexMode = Linear;		// ���� ����

		AlphaBlendEnable = TRUE;
		SrcBlend  = SRCCOLOR;
		DestBlend = INVSRCCOLOR;
		CullMode  = NONE;

	    AddressU[0] = Wrap;
	    AddressV[0] = Wrap;
	}

	pass p3
	{
		VertexShader = compile vs_1_1 VS_SkinBasic();
		PixelShader  = NULL;
		
		FogEnable = true;			// ���׻��
        FogVertexMode = Linear;		// ���� ����
	}

	pass p4
	{
		VertexShader = compile vs_1_1 VS_SkinSPEC_ENV();
		PixelShader  = compile ps_2_0 PS_SkinSPEC_ENV();

		FogEnable = true;			// ���׻��
        FogVertexMode = Linear;		// ���� ����

	    AddressU[0] = Wrap;
	    AddressV[0] = Wrap;
	}

	pass p5
	{
		VertexShader = compile vs_1_1 VS_SkinSPEC_ENV();
		PixelShader  = compile ps_2_0 PS_SkinSPEC_ENV_AHPHA();
		
		FogEnable = true;			// ���׻��
        FogVertexMode = Linear;		// ���� ����
		CullMode  = NONE;

		AlphaBlendEnable = TRUE;
		SrcBlend = SRCCOLOR;
		DestBlend = INVSRCCOLOR;
	}

	pass p6
	{
		VertexShader = compile vs_1_1 VSGlow();
        PixelShader  = compile ps_2_0 PSGlow();
        
        // no texture
        Texture[0] = NULL;

        // enable alpha blending
        AlphaBlendEnable = TRUE;
        SrcBlend         = ONE;
        DestBlend        = ONE;

        // set up texture stage states to use the diffuse color
        ColorOp[0]   = SELECTARG2;
        ColorArg2[0] = DIFFUSE;
        AlphaOp[0]   = SELECTARG2;
        AlphaArg2[0] = DIFFUSE;

        ColorOp[1]   = DISABLE;
        AlphaOp[1]   = DISABLE;

		CullMode  = NONE;
	}

}
