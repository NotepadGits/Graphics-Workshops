#pragma once
//TODO

///Window
//fullscreen				--complete
//moved/resized				-- Complete
//aspect ratio is maintained - Complete

///geometry
//background with picture
//world boundries
//player can shoot
//animation when bubble hit

///game
//bubbles cant leave the screen
//ubbles are desroyed when shot
//player has 3 lives and shown on screen
//other gameplay features added (bubbles spawn 2 smaller bubbles)

///colours and textures
//geometry arnt same colour
//player has texture
//bubbles and background are textured
//transparency
//textured destruction


///shaders
//shaders are used
//shaders are in their own files?????
//uniforms are used in shaders
// bubbles have random hue
// texture/geometry are maniplulated by shader

// https://krazydad.com/tutorials/makecolors.php

// https://realtimevfx.com/t/frost-grenade/6047

// https://www.iquilezles.org/www/articles/sphereshadow/sphereshadow.htm

// https://thebookofshaders.com/

// https://github.com/stackgl/shader-school

/// fragment

//precision highp float;
//uniform float time;
//uniform vec2 resolution;
//varying vec3 fPosition;
//varying vec3 fNormal;
//
//void main()
//{
//
//	float r = 1.0 + (sin(time * 20.0 + 0.0) / 2.0);
//	float g = 1.0 + (sin(time * 20.0 + 2.0) / 2.0);
//	float b = 1.0 + (sin(time * 20.0 + 4.0) / 2.0);
//
//	vec4 col = vec4(r, g, b, 1.0);
//
//	gl_FragColor = col;
//}


/// vertex
//
//precision highp float;
//attribute vec3 position;
//attribute vec3 normal;
//uniform mat3 normalMatrix;
//uniform mat4 modelViewMatrix;
//uniform mat4 projectionMatrix;
//varying vec3 fNormal;
//varying vec3 fPosition;
//
//void main()
//{
//	fNormal = normalize(normalMatrix * normal);
//	vec4 pos = modelViewMatrix * vec4(position, 1.0);
//	fPosition = pos.xyz;
//	gl_Position = projectionMatrix * pos;
//}