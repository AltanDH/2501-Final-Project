// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;

// Property to check for grayscale necessity (values are 0 & 1)
uniform int ghost;

void main()
{
    // Sample texture
    vec4 color = texture2D(onetex, uv_interp);

    // Set the color while checking for ghost mode
    if (ghost == 1) {
        // Calculate grayscale value using 'luminance formula'
        float gray = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;

        gl_FragColor = vec4(gray, gray, gray, color.a);
    }
    else {
        gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    }

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
