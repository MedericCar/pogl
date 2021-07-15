#version 450

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float time;


int p[512] = {
  151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
  151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

float fade(float v)
{
  return (v * v * v * (v * (v * 6.0 - 15.0) + 10.0));
}

// Pseudo-random direction : from first 4 bits of hash into one of 12 grad dirs
float grad(int hash, float x, float y, float z)
{
    int h = hash & 15;
    float u = (h < 8) ? x : y;
    float v = (h < 4) ? y : ((h == 12) || (h == 14)) ? x : z;
    return (((h&1) == 1) ? -u : u) + (((h&2) == 1)? -v : v);
}

float noise(float x, float y, float z)
{
  int ix0 = int(floor(x)) & 255;
  int ix1 = (ix0 + 1) & 255;
  float fx0 = x - float(ix0);
  float fx1 = fx0 - 1.0;

  int iy0 = int(floor(y)) & 255;
  int iy1 = (iy0 + 1) & 255;
  float fy0 = y - float(iy0);
  float fy1 = fy0 - 1.0;

  int iz0 = int(floor(z)) & 255;
  int iz1 = (iz0 + 1) & 255;
  float fz0 = z - float(iz0);
  float fz1 = fz0 - 1.0;

  float u = fade(fx0);
  float v = fade(fy0);
  float w = fade(fz0);

  // Hash coordinates of cube corners
  int A =  p[ix0    ] + iy0;
  int B =  p[ix0 + 1] + iy0;
  int AA = p[A     ] + iz0;
  int BA = p[B     ] + iz0;
  int AB = p[A  + 1] + iz0;
  int BB = p[B  + 1] + iz0;

  int c000_index = p[p[p[ix0] + iy0] + iz0];
  int c100_index = p[p[p[ix1] + iy0] + iz0];
  int c010_index = p[p[p[ix0] + iy1] + iz0];
  int c110_index = p[p[p[ix1] + iy1] + iz0];
  int c001_index = p[p[p[ix0] + iy0] + iz1];
  int c101_index = p[p[p[ix1] + iy0] + iz1];
  int c011_index = p[p[p[ix0] + iy1] + iz1];
  int c111_index = p[p[p[ix1] + iy1] + iz1];

  //float nxy0 = grad(p[ix0 + p[iy0 + p[iz0]]], fx0, fy0, fz0);
  //float nxy1 = grad(p[ix0 + p[iy0 + p[iz1]]], fx0, fy0, fz1);
  //float nx0 = mix( w, nxy0, nxy1 );

  //nxy0 = grad(p[ix0 + p[iy1 + p[iz0]]], fx0, fy1, fz0);
  //nxy1 = grad(p[ix0 + p[iy1 + p[iz1]]], fx0, fy1, fz1);
  //float nx1 = mix( w, nxy0, nxy1 );

  //float n0 = mix( v, nx0, nx1 );

  //nxy0 = grad(p[ix1 + p[iy0 + p[iz0]]], fx1, fy0, fz0);
  //nxy1 = grad(p[ix1 + p[iy0 + p[iz1]]], fx1, fy0, fz1);
  //nx0 = mix( w, nxy0, nxy1 );

  //nxy0 = grad(p[ix1 + p[iy1 + p[iz0]]], fx1, fy1, fz0);
  //nxy1 = grad(p[ix1 + p[iy1 + p[iz1]]], fx1, fy1, fz1);
  //nx1 = mix( w, nxy0, nxy1 );

  //float n1 = mix( v, nx0, nx1 );
  
  //return 0.936f * ( mix( u, n0, n1 ) );

  return mix(
    mix(
      mix(
        grad(c000_index, fx0  , fy0  , fz0  ),
        grad(c100_index, fx0-1, fy0  , fz0  ),
        u
      ),
      mix(
        grad(c010_index, fx0  , fy0-1, fz0  ),
        grad(c110_index, fx0-1, fy0-1, fz0  ),
        u
      ),
      v
    ),
    mix(
      mix(
        grad(c001_index, fx0  , fy0  , fz0-1),
        grad(c101_index, fx0-1, fy0  , fz0-1),
        u
      ),
      mix(
        grad(c011_index, fx0  , fy0-1, fz0-1),
        grad(c111_index, fx0-1, fy0-1, fz0-1),
        u
      ),
      v
    ),
    w
  );
}

void main()
{
    float ambientStrength = 0.2;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(reflect(-lightDir, norm), viewDir), 0.0), 32);

    float n = noise(gl_FragCoord.x * 0.01, gl_FragCoord.y*0.01, time);
    //FragColor = vec4((ambientStrength + diff + spec) * lightColor * n, 1.0);
    FragColor = vec4(n, n, n, 1.0);
} 
