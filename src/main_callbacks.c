#include "gui_registry.h"
#include <SDL3/SDL_events.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include "runtime.h"
#include "gui.h"
#include "menu.h"

/*
// the vertex input layout
struct Vertex
{
    float x, y, z;      //vec3 position
    float r, g, b, a;   //vec4 color
};

// a list of vertices
static struct Vertex vertices[] = {
    {0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},     // top vertex
    {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},   // bottom left vertex
    {0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f}     // bottom right vertex
};

struct UniformBuffer
{
    float time;
    // you can add other properties here
};

static struct UniformBuffer timeUniform = {0};

SDL_Window* window;
SDL_GPUDevice* device;
SDL_GPUBuffer* vertex_buffer;
SDL_GPUTransferBuffer* transfer_buffer;
SDL_GPUGraphicsPipeline* graphics_pipeline;
*/

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    init_runtime((runtime_t **)appstate);

    runtime_t *runtime = *appstate;

    runtime->active_context = CONTEXT_MENU;



    /*
    window = SDL_CreateWindow("ur gay", 960, 540, 0);
    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, "vulkan");

    SDL_ClaimWindowForGPUDevice(device, window);

    size_t spirv_size_vert;
    void* vertexCode = SDL_LoadFile("build/shaders/test.vert.spv", &spirv_size_vert);

    // create the vertex shader
    SDL_GPUShaderCreateInfo vertexInfo = {0};
    vertexInfo.code = (Uint8*)vertexCode;
    vertexInfo.code_size = spirv_size_vert;
    vertexInfo.entrypoint = "main";
    vertexInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
    vertexInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
    vertexInfo.num_samplers = 0;
    vertexInfo.num_storage_buffers = 0;
    vertexInfo.num_storage_textures = 0;
    vertexInfo.num_uniform_buffers = 0;
    SDL_GPUShader* vertexShader = SDL_CreateGPUShader(device, &vertexInfo);

    SDL_free(vertexCode);

    size_t fragmentCodeSize;
    void* fragmentCode = SDL_LoadFile("build/shaders/test.frag.spv", &fragmentCodeSize);

    //create the fragment shader
    SDL_GPUShaderCreateInfo fragmentInfo = {0};
    fragmentInfo.code = (Uint8*)fragmentCode;
    fragmentInfo.code_size = fragmentCodeSize;
    fragmentInfo.entrypoint = "main";
    fragmentInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
    fragmentInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    fragmentInfo.num_samplers = 0;
    fragmentInfo.num_storage_buffers = 0;
    fragmentInfo.num_storage_textures = 0;
    fragmentInfo.num_uniform_buffers = 1;

    SDL_GPUShader* fragmentShader = SDL_CreateGPUShader(device, &fragmentInfo);

    SDL_free(fragmentCode);

    // create the graphics pipeline
    SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {0};
    pipelineInfo.vertex_shader = vertexShader;
    pipelineInfo.fragment_shader = fragmentShader;
    pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

    // describe the vertex buffers
    SDL_GPUVertexBufferDescription vertex_bufferDesctiptions[1];
    vertex_bufferDesctiptions[0].slot = 0;
    vertex_bufferDesctiptions[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    vertex_bufferDesctiptions[0].instance_step_rate = 0;
    vertex_bufferDesctiptions[0].pitch = sizeof(struct Vertex);

    pipelineInfo.vertex_input_state.num_vertex_buffers = 1;
    pipelineInfo.vertex_input_state.vertex_buffer_descriptions = vertex_bufferDesctiptions;

    // describe the vertex attribute
    SDL_GPUVertexAttribute vertexAttributes[2];

    // a_position
    vertexAttributes[0].buffer_slot = 0;
    vertexAttributes[0].location = 0;
    vertexAttributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    vertexAttributes[0].offset = 0;

    // a_color
    vertexAttributes[1].buffer_slot = 0;
    vertexAttributes[1].location = 1;
    vertexAttributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
    vertexAttributes[1].offset = sizeof(float) * 3;

    pipelineInfo.vertex_input_state.num_vertex_attributes = 2;
    pipelineInfo.vertex_input_state.vertex_attributes = vertexAttributes;

    // describe the color target
    SDL_GPUColorTargetDescription colorTargetDescriptions[1] = {0};
    colorTargetDescriptions[0] = (SDL_GPUColorTargetDescription){0};
    colorTargetDescriptions[0].format = SDL_GetGPUSwapchainTextureFormat(device, window);

    pipelineInfo.target_info.num_color_targets = 1;
    pipelineInfo.target_info.color_target_descriptions = colorTargetDescriptions;

    // create the pipeline
    graphics_pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);

    // we don't need to store the shaders after creating the pipeline
    SDL_ReleaseGPUShader(device, vertexShader);
    SDL_ReleaseGPUShader(device, fragmentShader);

    // create the vertex buffer
    SDL_GPUBufferCreateInfo bufferInfo = {0};
    bufferInfo.size = sizeof(vertices);
    bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    vertex_buffer = SDL_CreateGPUBuffer(device, &bufferInfo);

    // create a transfer buffer to upload to the vertex buffer
    SDL_GPUTransferBufferCreateInfo transferInfo = {0};
    transferInfo.size = sizeof(vertices);
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    // fill the transfer buffer
    struct Vertex* data = (struct Vertex*)SDL_MapGPUTransferBuffer(device, transfer_buffer, false);

    SDL_memcpy(data, (void*)vertices, sizeof(vertices));

    // data[0] = vertices[0];
    // data[1] = vertices[1];
    // data[2] = vertices[2];

    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

    // start a copy pass
    SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);

    // where is the data
    SDL_GPUTransferBufferLocation location = {0};
    location.transfer_buffer = transfer_buffer;
    location.offset = 0;

    // where to upload the data
    SDL_GPUBufferRegion region = {0};
    region.buffer = vertex_buffer;
    region.size = sizeof(vertices);
    region.offset = 0;

    // upload the data
    SDL_UploadToGPUBuffer(copyPass, &location, &region, true);

    // end the copy pass
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(commandBuffer);
    */




    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    runtime_t *runtime = appstate;

    switch (runtime->active_context)
    {
        case CONTEXT_MENU:
        draw_frame_menu(runtime->menu, runtime->gui_renderer);
        break;

        default:
        return SDL_APP_CONTINUE;
    }

    /*
    // acquire the command buffer
    SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(device);

    // get the swapchain texture
    SDL_GPUTexture* swapchainTexture;
    Uint32 width, height;
    SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, window, &swapchainTexture, &width, &height);

    // end the frame early if a swapchain texture is not available
    if (swapchainTexture == NULL)
    {
        // you must always submit the command buffer
        SDL_SubmitGPUCommandBuffer(commandBuffer);
        return SDL_APP_CONTINUE;
    }

    // create the color target
    SDL_GPUColorTargetInfo colorTargetInfo = {0};
    colorTargetInfo.clear_color = (SDL_FColor){240/255.0f, 240/255.0f, 240/255.0f, 255/255.0f};
    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
    colorTargetInfo.texture = swapchainTexture;

    // begin a render pass
    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, NULL);

    // bind the pipeline
    SDL_BindGPUGraphicsPipeline(renderPass, graphics_pipeline);

    // bind the vertex buffer
    SDL_GPUBufferBinding bufferBindings[1];
    bufferBindings[0].buffer = vertex_buffer;
    bufferBindings[0].offset = 0;

    SDL_BindGPUVertexBuffers(renderPass, 0, bufferBindings, 1);

    // update the time uniform
    timeUniform.time = SDL_GetTicksNS() / 1e9f;
    SDL_PushGPUFragmentUniformData(commandBuffer, 0, &timeUniform, sizeof(struct UniformBuffer));

    // issue a draw call
    SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);

    // end the render pass
    SDL_EndGPURenderPass(renderPass);

    // submit the command buffer
    SDL_SubmitGPUCommandBuffer(commandBuffer);
    */

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    runtime_t *runtime = appstate;

    switch (event->type)
    {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        return SDL_APP_SUCCESS;
        break;

        case SDL_EVENT_WINDOW_RESIZED:
        SDL_GetWindowSize(
            runtime->window,
            &runtime->window_size_x,
            &runtime->window_size_y
        );

        update_menu_size(runtime->menu, runtime->window_size_x, runtime->window_size_y);
        break;
    }

    switch (runtime->active_context) {
        case CONTEXT_MENU:
        input_handler_menu(runtime->menu, event, runtime);
        if (runtime->should_quit)
        {
            return SDL_APP_SUCCESS;
        }
        break;

        default:
        break;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    runtime_t *runtime = appstate;

    cleanup_runtime(runtime);

    /*
    // release buffers
    SDL_ReleaseGPUBuffer(device, vertex_buffer);
    SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);

    // release the pipeline
    SDL_ReleaseGPUGraphicsPipeline(device, graphics_pipeline);

    // destroy the GPU device
    SDL_DestroyGPUDevice(device);

    // destroy the window
    SDL_DestroyWindow(window);
    */
}