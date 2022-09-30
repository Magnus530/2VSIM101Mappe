#version 410 core
out vec4 fragColor;         //The final color

in vec3 normalTransposed;   //Normal of fragment, already transposed
in vec3 fragmentPosition;
in vec2 UV;       //for textures

uniform sampler2D textureSampler;

uniform float ambientStrength = 0.1;

uniform vec3 lightPosition;
uniform vec3 lightColor = vec3(.3, .3, 1.);  //blueish
uniform float lightStrength = 0.5;
uniform float specularStrength = 0.5;
uniform int specularExponent = 32;

uniform vec3 objectColor = vec3(1.0, 1.0, 1.0); //white

uniform vec3 cameraPosition = vec3(0, 0, 0);;

void main() {
    //ambient
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    float angleFactor = max(dot(normalCorrected, lightDirection), 0.0);
    vec3 diffuse = angleFactor * objectColor * lightColor * lightStrength;

    //specular
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    float spec = 0.0;
    if (angleFactor > 0.0)     //calculations only needed if diffuse is above 0.0
    {
        vec3 reflectDirection = reflect(-lightDirection, normalCorrected);
        spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
    }
    vec3 specular = spec * lightColor * specularStrength;

    vec3 result = ambient + diffuse + specular;

    fragColor = vec4(result, 1.0) * texture(textureSampler, UV);

    if (fragmentPosition.y > 3.85f && fragmentPosition.y < 4.f)
    {
        fragColor = vec4(vec3(1.f, 0.f, 0.f), 1.f);
    }
    else if (fragmentPosition.y > 2.85f && fragmentPosition.y < 3.f)
    {
        fragColor = vec4(vec3(1.f, 0.f, 0.f), 1.f);
    }


//    fragColor = vec4(result, 1.0); // * texture(textureSampler, UV); To use lighting on plainshader instead of texture.
}
