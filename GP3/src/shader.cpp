#include "pch.h"
#include "shader.h"
#include "openGl.h"
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram( )
{
	_program = glCreateProgram( );
	glErrorCheck( );
}
ShaderProgram::~ShaderProgram( )
{
	if ( _program != 0 )
	{
		glDeleteProgram( _program );
	}
}
void ShaderProgram::addShader( Shader* shader )
{
	glAttachShader( _program, shader->_shader );
}
void ShaderProgram::use( )
{
	glLinkProgram( _program );
	int linkStatus;
	glGetProgramiv( _program, GL_LINK_STATUS, &linkStatus );
	if ( linkStatus == GL_TRUE )
	{
		glUseProgram( _program );
	}
	else
	{
		ERR( "Unable to link shader program." );
	}
	glErrorCheck( );
}
void ShaderProgram::setProjection( const glm::mat4& projection )
{
	GLint projectionMatrix = glGetUniformLocation( _program, "projectionMatrix" );
	if ( projectionMatrix != -1 )
	{
		glUniformMatrix4fv( projectionMatrix, 1, GL_FALSE, glm::value_ptr( projection ) );
	}
	glErrorCheck( );
}
void ShaderProgram::setView( const glm::mat4& view )
{
	GLint viewMatrix = glGetUniformLocation( _program, "viewMatrix" );
	if ( viewMatrix != -1 )
	{
		glUniformMatrix4fv( viewMatrix, 1, GL_FALSE, glm::value_ptr( view ) );
	}
	glErrorCheck( );
}
void ShaderProgram::setWorld( const glm::mat4& world )
{
	GLint worldMatrix = glGetUniformLocation( _program, "worldMatrix" );
	if ( worldMatrix != -1 )
	{
		glUniformMatrix4fv( worldMatrix, 1, GL_FALSE, glm::value_ptr( world ) );
	}
	glErrorCheck( );
}
void ShaderProgram::setAmbientLightColor( const glm::vec3& color )
{
	GLint ambientLightColor = glGetUniformLocation( _program, "ambientLightColor" );
	if ( ambientLightColor != -1 )
	{
		glUniform3f( ambientLightColor, color.r, color.g, color.b );
	}
	glErrorCheck( );
}
void ShaderProgram::setDiffuseLightColor( const glm::vec3& color )
{
	GLint diffuseLightColor = glGetUniformLocation( _program, "diffuseLightColor" );
	if ( diffuseLightColor != -1 )
	{
		glUniform3f( diffuseLightColor, color.r, color.g, color.b );
	}
	glErrorCheck( );
}
void ShaderProgram::setLightDirection( const glm::vec3& dir )
{
	GLint lightDirection = glGetUniformLocation( _program, "lightDirection" );
	if ( lightDirection != -1 )
	{
		glUniform3f( lightDirection, dir.x, dir.y, dir.z );
	}
	glErrorCheck( );
}
void ShaderProgram::setTextureUnit( const unsigned textureUnit )
{
	GLint samplerLoc = glGetUniformLocation( _program, "gSampler" );
	if ( samplerLoc != -1 )
	{
		glUniform1i( samplerLoc, textureUnit );
	}
	glErrorCheck( );
}

Shader::Shader( String&& filename, GLenum type )
{
	// read shader file
	FILE* file = fopen( filename.c_str( ), "rt" );
	if ( file == nullptr )
	{
		ERR( "Unable to open shader " << filename << "." );
		glErrorCheck( );
	}
	// get text lines in shader file
	std::vector<String> lines;
	char line[255];
	while ( fgets( line, 255, file ) )
	{
		lines.push_back( line );
	}
	fclose( file );
	// copy text lines into char**
	const char** linesC = new const char*[lines.size( )];
	for ( unsigned i = 0; i < lines.size( ); ++i )
	{
		linesC[i] = lines[i].c_str( );
	}
	_shader = glCreateShader( type );
	glShaderSource( _shader, lines.size( ), linesC, NULL );
	glCompileShader( _shader );
	delete[] linesC;
	GLint compiled;
	glGetShaderiv( _shader, GL_COMPILE_STATUS, &compiled );
	if ( compiled == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetShaderiv( _shader, GL_INFO_LOG_LENGTH, &maxLength );
		// The maxLength includes the NULL character
		char* errorLog = new char[maxLength];
		glGetShaderInfoLog( _shader, maxLength, &maxLength, &errorLog[0] );
		glDeleteShader( _shader );
		ERR( "Unable to compile shader " << filename << ". " << errorLog );
		glErrorCheck( );
	}
}
Shader::~Shader( )
{
	if ( _shader != 0 )
	{
		glDeleteShader( _shader );
	}
}