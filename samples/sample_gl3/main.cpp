#include <im3d.h>
#include <im3d_newframe.h>
#include "../win32_window.h"
#include "../orbit_camera.h"
#include <gl3_renderer.h>

int main(int, char **)
{
    Win32Window window;
    if (!window.Create(640, 480, L"sample_gl3"))
    {
        return 1;
    }

    OrbitCamera camera;

    if (!GL3_Initialize(window.GetHandle()))
    {
        return 1;
    }
    if (!Im3d_GL3_Initialize())
    {
        return 2;
    }

    // Unified gizmo operates directly on a 4x4 matrix using the context-global gizmo modes.
    //static Im3d::Mat4 transform(1.0f);
    static std::array<float, 16> transform = {
        1,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        1,
    };

    while (window.IsRunning())
    {
        int w, h;
        auto mouse = window.GetMouseState();
        std::tie(w, h) = window.GetSize();
        camera.SetScreenSize((float)w, (float)h);
        camera.MouseInput(mouse);
        camera.state.CalcViewProjection();

        Im3dGui_NewFrame(&camera.state, &mouse, 0, -1);
        Im3d::Gizmo("GizmoUnified", transform.data());
        Im3d::EndFrame();

        GL3_NewFrame(w, h);
        {
            GL3_DrawTeapot(camera.state.viewProjection.data(), transform.data());
            Im3d_GL3_Draw(camera.state.viewProjection.data(), w, h, Im3d::GetDrawLists(), Im3d::GetDrawListCount());
        }
        GL3_EndFrame();
    }

    Im3d_GL3_Finalize();
    GL3_Finalize();

    return 0;
}
