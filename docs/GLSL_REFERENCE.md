# GLSL Shaders

## Definitions

- GLSL - OpenGL Shading Language
  - OpenGL -> Open Graphics Language
  - Open Graphics Library Shading Language

## Notes on vec

- swizzling and vec instantiation
  <https://thebookofshaders.com/appendix/04/>
- swizzling basics
  <https://imalogic.com/blog/2018/05/24/glsl-optimizations/>
- Online GLSL tools seem to be inconsistent in swizzle behavior
  - TODO examples

## Uniform Variables (Uniforms)

variables accessible from all shader threads

### twigl

uniforms are mode-specific

- classic  
"resolution", "mouse", "time", "frame", "backbuffer"
- geek  
"r", "m", "t", "f", "b"
- geeker  
"r", "m", "t", "f", "b" are accessible, no need to declare

## Language Editors

<http://editor.thebookofshaders.com/>  
<https://twigl.app/>

## Run twigl locally

```sh
git clone https://github.com/doxas/twigl.git
cd twigl
npm install
# required because of code: 'ERR_OSSL_EVP_UNSUPPORTED'
# https://stackoverflow.com/questions/75959563/node-js-err-ossl-evp-unsupported-error-when-running-npm-run-start
export NODE_OPTIONS=--openssl-legacy-provider
npm start
```

## Shader Examples

[GLSL_EXAMPLES](./GLSL_EXAMPLES.md)
