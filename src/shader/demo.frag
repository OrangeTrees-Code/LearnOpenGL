#version 460 core


flat in int VertexID;
out vec4 FragColor;

void main(){
    // if(VertexID % 2 ==0){
    //     FragColor = vec4(1,0,0,1);
    // }
    // else if(VertexID % 3 == 0){
    //     FragColor = vec4(0,1,0,1);
    // }
    // else{
    //     FragColor = vec4(0,0,1,1);
    // }

    // if(gl_FragCoord.x < 1280)
    //     FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    // else
    //     FragColor = vec4(0.0, 1.0, 0.0, 1.0);  

    if(gl_FrontFacing)
        FragColor = vec4(1,0,0,1);
    else
        FragColor = vec4(0,0,1,1);
}

// #version 420 core // 注意GLSL的版本！
// out vec4 FragColor;
// layout (depth_greater) out float gl_FragDepth;

// void main()
// {             
//     FragColor = vec4(1.0);
//     gl_FragDepth = gl_FragCoord.z + 0.1;
// }  