/* This file has been generated automaticaly by extglgen */

/*
	extgl - OpenGL Extension Loading Library
	Copyright (c) 2004, Trenkwalder Markus
	All rights reserved. 
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	
	- Redistributions of source code must retain the above copyright notice,
	  this list of conditions and the following disclaimer.
	  
	- Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	  
	- Neither the name of extgl's copyright owner nor the names
	  of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.
	  
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	
	Contact info:
	email: trenki2@gmx.net web: trenki.50free.org
*/

/* Some parts are */

/*
** License Applicability. Except to the extent portions of this file are
** made subject to an alternative license as permitted in the SGI Free
** Software License B, Version 1.1 (the "License"), the contents of this
** file are subject only to the provisions of the License. You may not use
** this file except in compliance with the License. You may obtain a copy
** of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
** Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
** 
** http://oss.sgi.com/projects/FreeB
** 
** Note that, as provided in the License, the Software is distributed on an
** "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
** DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
** CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
** PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
** 
** Original Code. The Original Code is: OpenGL Sample Implementation,
** Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
** Inc. The Original Code is Copyright (c) 1991-2002 Silicon Graphics, Inc.
** Copyright in any portions created by third parties is as indicated
** elsewhere herein. All Rights Reserved.
** 
** Additional Notice Provisions: This software was created using the
** OpenGL(R) version 1.2.1 Sample Implementation published by SGI, but has
** not been independently verified as being compliant with the OpenGL(R)
** version 1.2.1 Specification.
*/	

#include "extgl.h"

#include <assert.h>

/* If you want to use SDL use the following */
/*
#include "SDL.h" 
#define _GetProcAddress SDL_GL_GetProcAddress 
*/

#define _GetProcAddress wglGetProcAddress

#ifdef __cplusplus
extern "C" {
#endif


/* GL_ARB_occlusion_query */
static void APIENTRY InitGenQueriesARB(GLsizei n, GLuint *ids)
{
	PFNGLGENQUERIESARBPROC proc;
	proc = (PFNGLGENQUERIESARBPROC)_GetProcAddress("glGenQueriesARB");
	assert(proc != NULL);
	glGenQueriesARB = proc;
	glGenQueriesARB(n, ids);
}

static void APIENTRY InitDeleteQueriesARB(GLsizei n, const GLuint *ids)
{
	PFNGLDELETEQUERIESARBPROC proc;
	proc = (PFNGLDELETEQUERIESARBPROC)_GetProcAddress("glDeleteQueriesARB");
	assert(proc != NULL);
	glDeleteQueriesARB = proc;
	glDeleteQueriesARB(n, ids);
}

static GLboolean APIENTRY InitIsQueryARB(GLuint id)
{
	PFNGLISQUERYARBPROC proc;
	proc = (PFNGLISQUERYARBPROC)_GetProcAddress("glIsQueryARB");
	assert(proc != NULL);
	glIsQueryARB = proc;
	return glIsQueryARB(id);
}

static void APIENTRY InitBeginQueryARB(GLenum target, GLuint id)
{
	PFNGLBEGINQUERYARBPROC proc;
	proc = (PFNGLBEGINQUERYARBPROC)_GetProcAddress("glBeginQueryARB");
	assert(proc != NULL);
	glBeginQueryARB = proc;
	glBeginQueryARB(target, id);
}

static void APIENTRY InitEndQueryARB(GLenum target)
{
	PFNGLENDQUERYARBPROC proc;
	proc = (PFNGLENDQUERYARBPROC)_GetProcAddress("glEndQueryARB");
	assert(proc != NULL);
	glEndQueryARB = proc;
	glEndQueryARB(target);
}

static void APIENTRY InitGetQueryivARB(GLenum target, GLenum pname, GLint *params)
{
	PFNGLGETQUERYIVARBPROC proc;
	proc = (PFNGLGETQUERYIVARBPROC)_GetProcAddress("glGetQueryivARB");
	assert(proc != NULL);
	glGetQueryivARB = proc;
	glGetQueryivARB(target, pname, params);
}

static void APIENTRY InitGetQueryObjectivARB(GLuint id, GLenum pname, GLint *params)
{
	PFNGLGETQUERYOBJECTIVARBPROC proc;
	proc = (PFNGLGETQUERYOBJECTIVARBPROC)_GetProcAddress("glGetQueryObjectivARB");
	assert(proc != NULL);
	glGetQueryObjectivARB = proc;
	glGetQueryObjectivARB(id, pname, params);
}

static void APIENTRY InitGetQueryObjectuivARB(GLuint id, GLenum pname, GLuint *params)
{
	PFNGLGETQUERYOBJECTUIVARBPROC proc;
	proc = (PFNGLGETQUERYOBJECTUIVARBPROC)_GetProcAddress("glGetQueryObjectuivARB");
	assert(proc != NULL);
	glGetQueryObjectuivARB = proc;
	glGetQueryObjectuivARB(id, pname, params);
}




/* GL_ARB_occlusion_query */
PFNGLGENQUERIESARBPROC glGenQueriesARB = InitGenQueriesARB;
PFNGLDELETEQUERIESARBPROC glDeleteQueriesARB = InitDeleteQueriesARB;
PFNGLISQUERYARBPROC glIsQueryARB = InitIsQueryARB;
PFNGLBEGINQUERYARBPROC glBeginQueryARB = InitBeginQueryARB;
PFNGLENDQUERYARBPROC glEndQueryARB = InitEndQueryARB;
PFNGLGETQUERYIVARBPROC glGetQueryivARB = InitGetQueryivARB;
PFNGLGETQUERYOBJECTIVARBPROC glGetQueryObjectivARB = InitGetQueryObjectivARB;
PFNGLGETQUERYOBJECTUIVARBPROC glGetQueryObjectuivARB = InitGetQueryObjectuivARB;


int egl_hasExtension(const char* ext) {
  int result;
  char tmp[80]; /* 80 should be safe */
  const char *extensions = (const char*)glGetString(GL_EXTENSIONS);
  
  strcpy(tmp, ext);
  strcat(tmp, " ");
	
  result = strstr(extensions, tmp) != NULL;
	
  strcpy(tmp, " ");
  strcat(tmp, ext);
  
  result = result || ( strstr(extensions, tmp) != NULL ) ;
  
  return result;
}

int EGL_HAS_GL_ARB_occlusion_query = 0;


static int egl_Init_GL_ARB_occlusion_query(void) {
	int result = 1;

	result = result && (glGenQueriesARB = (PFNGLGENQUERIESARBPROC)_GetProcAddress("glGenQueriesARB"));
	result = result && (glDeleteQueriesARB = (PFNGLDELETEQUERIESARBPROC)_GetProcAddress("glDeleteQueriesARB"));
	result = result && (glIsQueryARB = (PFNGLISQUERYARBPROC)_GetProcAddress("glIsQueryARB"));
	result = result && (glBeginQueryARB = (PFNGLBEGINQUERYARBPROC)_GetProcAddress("glBeginQueryARB"));
	result = result && (glEndQueryARB = (PFNGLENDQUERYARBPROC)_GetProcAddress("glEndQueryARB"));
	result = result && (glGetQueryivARB = (PFNGLGETQUERYIVARBPROC)_GetProcAddress("glGetQueryivARB"));
	result = result && (glGetQueryObjectivARB = (PFNGLGETQUERYOBJECTIVARBPROC)_GetProcAddress("glGetQueryObjectivARB"));
	result = result && (glGetQueryObjectuivARB = (PFNGLGETQUERYOBJECTUIVARBPROC)_GetProcAddress("glGetQueryObjectuivARB"));

	return result;
}



int egl_InitExtensions(void) {
	int result = 1;

	result = result & (EGL_HAS_GL_ARB_occlusion_query = egl_Init_GL_ARB_occlusion_query());

	return result;
}


#ifdef __cplusplus
}
#endif

