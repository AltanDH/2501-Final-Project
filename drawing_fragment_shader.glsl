// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;

// Value for how much of the bar to fill (0.0 - 1.0)
uniform float fill_val;
// Filling color
uniform vec4 color;

void main()
{
    vec4 final_color;

    // If the fragment is not on the shape, make it black
    if (uv_interp.x > fill_val) {
        final_color = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else {
        final_color = color;
    }

    // Check for transparency
    if (color.a < 1.0) {
         discard;
    }

    // Set the fragment color by tinting the original texture
    final_color = texture2D(onetex, uv_interp) * final_color;
    gl_FragColor = final_color;
}
