//
//  IKeOpenGLRenderDevice.h
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __IKeOpenGLRenderDevice__
#define __IKeOpenGLRenderDevice__

#include "KeRenderDevice.h"

#ifdef __APPLE__
 #ifdef __MOBILE_OS__
  #include "SDL.h"
  #include <OpenGLES/ES2/gl.h>
  #include <OpenGLES/ES3/gl.h>
  #include <OpenGLES/ES2/glext.h>
  #include <OpenGLES/ES3/glext.h>
 #else
  #include <SDL2/SDL.h>
  #include <OpenGL/OpenGL.h>
  #include <OpenGL/gl3.h>
  #include <OpenGL/gl3ext.h>
 #endif
#else
#include <SDL.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#endif


/*
 * Constant buffer structure
 */
struct IKeOpenGLConstantBuffer : public IKeConstantBuffer
{
    virtual void Destroy();
    
    void*       buffer_data;    /* Constant buffer data */
    uint32_t    data_size;      /* Size of the data buffer */
    uint32_t*   locations;      /* Constant locations (for faux constant buffers) */
    uint32_t    location_count; /* Number of constant locations (for faux constant buffers) */
    uint32_t*   offsets;        /* Constant offsets (for faux constant buffers) */
    uint32_t    offset_count;   /* Number of offsets (for faux constant buffers) */
};

/*
 * Geometry buffer structure
 */
struct IKeOpenGLGeometryBuffer : public IKeGeometryBuffer
{
    virtual void Destroy();
    
    virtual void* MapData( uint32_t flags );
	virtual void* MapDataAsync( uint32_t flags );
    virtual void UnmapData( void* );
    virtual void UnmapDataAsync( void* );

    virtual bool SetVertexData( uint32_t offset, uint32_t size, void* ptr );
    virtual bool SetIndexData( uint32_t offset, uint32_t size, void* ptr );
	virtual void GetDesc( KeGeometryBufferDesc* desc );
    
    uint32_t vbo[2];		/* Vertex and index buffer */
    uint32_t vao;			/* Vertex array object */
    uint32_t vd_length;		/* Length of vertex data (in bytes) */
	uint32_t id_length;		/* Length of index data (in bytes) */
	uint32_t vertex_size;	/* Size of each vertex (in bytes) */
    uint32_t index_type;	/* Data type for index data */
    uint32_t lock_flags;	/* Buffer lock flags */
};

/*
 * Command list structure
 */
struct IKeOpenGLCommandList : public IKeCommandList 
{
	virtual void Destroy();
};

/*
 * GPU Program structure
 */
struct IKeOpenGLGpuProgram : public IKeGpuProgram
{
    virtual void Destroy();
	virtual void GetVertexAttributes( KeVertexAttribute* vertex_attributes );
    
    uint32_t program;       /* GPU program handle */
    uint32_t matrices[3];   /* Handles to the world, view, and projection matrices (respectively) */
	KeVertexAttribute* va;	/* Vertex attributes */
};

/*
 * Texture base structure
 */
struct IKeOpenGLTexture : public IKeTexture
{
    virtual void Destroy();
    
    virtual void* MapData( uint32_t flags );
    virtual void UnmapData( void* );
    
    virtual bool SetTextureData( KeTextureDesc* texture_data, void* pixels );
	virtual bool GetTextureDesc( KeTextureDesc* texture_desc );
    
    uint32_t handle;            /* Handle to the OpenGL texture */
	uint32_t mipmap;			
    uint32_t width, height;     /* Texture width/height */
    uint32_t depth;             /* Texture depth (for 3D and array textures) */
    uint32_t depth_format;      /* See glTexImageXD */
    uint32_t internal_format;   /* See glTexImageXD */
    uint32_t data_type;         /* Internal data type */
    uint32_t target;            /* OpenGL texture target */
};

/*
 * Rendertarget base structure 
 */
struct IKeOpenGLRenderTarget : public IKeRenderTarget
{
    virtual void Destroy();
    
	virtual void* MapData( uint32_t flags );
	virtual void UnmapData( void* );

	virtual bool GetTexture( IKeTexture** texture );
	virtual IKeTexture* GetTexture2();

    uint32_t    fbo;				/* Frame buffer object handle */
    uint32_t    depth_buffer;		/* Depth render buffer */
    uint32_t	stencil_buffer;		/* Stencil buffer */
    
    IKeTexture* texture;
};

/*
 * Palette base structure
 */
struct IKeOpenGLPalette : public IKePalette
{
    virtual void Destroy();
};

/*
 * GPU fence structure 
 */
struct IKeOpenGLFence : public IKeFence
{
    virtual void Destroy();
    
    virtual bool Insert();
    virtual bool Test();
    virtual void Block();
    virtual bool Valid();
    
	uint32_t	fence;		/* GL_NV_fence, GL_APPLE_fence */
	GLsync		sync;		/* GL_ARB_sync, GL_APPLE_sync */
    int         vendor;     /* Fence extension vendor to use */
};

/*
 * Render/Texture state structure
 */
struct IKeOpenGLRenderStateBuffer : public IKeRenderStateBuffer
{
    virtual void Destroy();
    
    KeState*    states;         /* OpenGL state values */
    int         state_count;    /* The number of states in this buffer */
};

struct IKeOpenGLTextureSamplerBuffer : public IKeTextureSamplerBuffer
{
	virtual void Destroy();

	KeState*    states;         /* OpenGL state values */
	int         state_count;    /* The number of states in this buffer */
};


/*
 * Render device base class
 */
class IKeOpenGLRenderDevice : public IKeRenderDevice
{
public:
    IKeOpenGLRenderDevice();
    IKeOpenGLRenderDevice( KeRenderDeviceDesc* renderdevice_desc );
    virtual ~IKeOpenGLRenderDevice();
    
public:
    /* Misc */
    virtual bool ConfirmDevice();
    virtual void GetDeviceDesc( KeRenderDeviceDesc* device_desc );
    virtual void GetDeviceCaps( KeRenderDeviceCaps* device_caps );
    
    /* General rendering stuff */
    virtual void SetClearColourFV( float* colour );
    virtual void SetClearColourUBV( uint8_t* colour );
    virtual void SetClearDepth( float depth );
	virtual void SetClearStencil( uint32_t stencil );
    virtual void ClearColourBuffer();
    virtual void ClearDepthBuffer();
    virtual void ClearStencilBuffer();
	virtual void Clear( uint32_t buffers );
    virtual void Swap();
    
    virtual void SetIMCacheSize( uint32_t cache_size );
    virtual bool CreateGeometryBuffer( void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, KeVertexAttribute* va, IKeGeometryBuffer** geometry_buffer );
    virtual void DeleteGeometryBuffer( IKeGeometryBuffer* geometry_buffer );
    virtual void SetGeometryBuffer( IKeGeometryBuffer* geometry_buffer );
    virtual bool CreateProgram( const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, KeVertexAttribute* vertex_attributes, IKeGpuProgram** gpu_program );
    virtual void DeleteProgram( IKeGpuProgram* gpu_program );
    virtual void SetProgram( IKeGpuProgram* gpu_program );
    virtual void SetProgramConstant1FV( const char* location, int count, float* value );
    virtual void SetProgramConstant2FV( const char* location, int count, float* value );
    virtual void SetProgramConstant3FV( const char* location, int count, float* value );
    virtual void SetProgramConstant4FV( const char* location, int count, float* value );
    virtual void SetProgramConstant1IV( const char* location, int count, int* value );
    virtual void SetProgramConstant2IV( const char* location, int count, int* value );
    virtual void SetProgramConstant3IV( const char* location, int count, int* value );
    virtual void SetProgramConstant4IV( const char* location, int count, int* value );
    virtual void GetProgramConstantFV( const char* location, float* value );
    virtual void GetProgramConstantIV( const char* location, int* value );
	virtual bool CreateConstantBuffer( uint32_t buffer_size, IKeConstantBuffer** constant_buffer );
	virtual void DeleteConstantBuffer( IKeConstantBuffer* constant_buffer );
	virtual bool SetConstantBufferData( void* data, IKeConstantBuffer* constant_buffer );
	virtual void SetVertexShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer );
	virtual void SetPixelShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer );
	virtual void SetGeometryShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer );
	virtual void SetTesselationShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer );
    virtual bool CreateTexture1D( uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL );
    virtual bool CreateTexture2D( uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL );
    virtual bool CreateTexture3D( uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL );
    virtual void DeleteTexture( IKeTexture* texture );
    virtual void SetTextureData1D( int offsetx, int width, int miplevel, void* pixels, IKeTexture* texture );
    virtual void SetTextureData2D( int offsetx, int offsety, int width, int height, int miplevel, void* pixels, IKeTexture* texture );
    virtual void SetTextureData3D( int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, IKeTexture* texture );
    virtual bool CreateRenderTarget( int width, int height, int depth, uint32_t flags, IKeRenderTarget** rendertarget );
    virtual void DeleteRenderTarget( IKeRenderTarget* rendertarget );
    virtual void BindRenderTarget( IKeRenderTarget* rendertarget );
    virtual void SetTexture( int stage, IKeTexture* texture );
	virtual bool CreateRenderStateBuffer( KeState* state_params, int state_count, IKeRenderStateBuffer** state_buffer );
	virtual bool CreateTextureSamplerBuffer( KeState* state_params, int state_count, IKeTextureSamplerBuffer** state_buffer );
	virtual bool SetRenderStateBuffer( IKeRenderStateBuffer* state_buffer );
	virtual bool SetTextureSamplerBuffer( int stage, IKeTextureSamplerBuffer* state_buffer );
	virtual void SetRenderStates( KeState* states );
	virtual void SetSamplerStates( int stage, KeState* states );
    virtual void DrawVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int first, int count, void* vertex_data );
    virtual void DrawIndexedVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int count, void* vertex_data, void* index_data );
    virtual void DrawIndexedVerticesRangeIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int start, int end, int count, void* vertex_data, void* index_data );
    virtual void DrawVertices( uint32_t primtype, uint32_t stride, int first, int count );
    virtual void DrawIndexedVertices( uint32_t primtype, uint32_t stride, int count );
    virtual void DrawIndexedVerticesRange( uint32_t primtype, uint32_t stride, int start, int end, int count );
    
    virtual bool GetFramebufferRegion( int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels );
    
    /* Matrix/viewport related */
    virtual void SetViewport( int x, int y, int width, int height );
    virtual void SetViewportV( int* viewport );
    virtual void GetViewport( int* x, int* y, int* width, int* height );
    virtual void GetViewportV( int* viewport );
    virtual void SetPerspectiveMatrix( float fov, float aspect, float near_z, float far_z );
    virtual void SetViewMatrix( const nv::matrix4f* view );
    virtual void SetWorldMatrix( const nv::matrix4f* world );
    virtual void SetModelviewMatrix( const nv::matrix4f* modelview );
    virtual void SetProjectionMatrix( const nv::matrix4f* projection );
	virtual void GetViewMatrix( nv::matrix4f* view );
    virtual void GetWorldMatrix( nv::matrix4f* world );
    virtual void GetModelviewMatrix( nv::matrix4f* modelview );
    virtual void GetProjectionMatrix( nv::matrix4f* projection );
    
    /* Synchronization */
    virtual void BlockUntilVerticalBlank();
    virtual void SetSwapInterval( int swap_interval );
    virtual int GetSwapInterval();
	virtual void BlockUntilIdle();
	virtual void Kick();
    virtual bool CreateFence( IKeFence** fence );
#if 0
	virtual bool InsertFence( IKeFence** fence );
	virtual bool TestFence( IKeFence* fence );
	virtual void BlockOnFence( IKeFence* fence );
	virtual void DeleteFence( IKeFence* fence );
	virtual bool IsFence( IKeFence* fence );
#endif
    
    /* Misc */
    virtual void GpuMemoryInfo( uint32_t* total_memory, uint32_t* free_memory );
    
private:    /* Private, internal use only */
    void PVT_ApplySamplerStates();
    void PVT_SetWorldViewProjectionMatrices();
    
protected:
    SDL_GLContext context;
    SDL_Window* window;
	int			major_version;
	int			minor_version;
	int			fence_vendor;
	void*		dd;
	KeState		samplers[8][16];
	int			dirty_samplers[8][16];
    uint32_t    im_cache_size;
    IKeGeometryBuffer* im_gb;
};


/*
 * Sprite factory class
 */
class IKeOpenGLSpriteFactory : public IKeSpriteFactory
{
public:
    IKeOpenGLSpriteFactory() {}
    IKeOpenGLSpriteFactory( IKeRenderDevice* renderdevice, KeVertexAttribute* vertex_attributes, uint32_t batch_size = KE_DEFAULT_BATCH_SIZE );
    virtual ~IKeOpenGLSpriteFactory();
    
public:
    virtual bool Initialized();
    //virtual void SetStates( IKeStateBuffer* state );
    virtual void SetProgram( IKeGpuProgram* program );
    virtual void SetTexture( IKeTexture* texture );
    virtual void SetVertexData( void* vertex_data, uint32_t offset, uint32_t vertex_data_size );
    virtual bool Lock( void** data );
    virtual void Unlock( void* data );
    
    virtual void Draw();
};

#endif /* defined(__IKeOpenGLRenderDevice__) */
