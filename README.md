# ProcSDF

![Logo](Assets/logo.png)

| Nope. This wasn't rendered in ProcSDF (yet :p) but in Blender. Hopefully we'll be able to render something like this in ProcSDF pretty soon. Godspeed, I guess.

---

# What is ProcSDF?

An **open-source**, **procedural**, **node-based**, **raymarched** 3D creation tool.

- It's open-source. Though this has started as a final year BTP project as a part of our CSE coursework, we plan on making it a community-driven project once the BTP is done with.

- It's procedural and node-based. Everything happens through nodes. The node workspace is your playground; add primitives, translate them, rotate them, perform operations and set up materials (TODO : materials :p) - all in the workspace.

- It's raymarched. Think raytracing, but more fun. Check out [this very fun blog](https://michaelwalczyk.com/blog-ray-marching.html) to know more about raymarching. Other than that [Inigo Quilez](https://iquilezles.org/)'s resources on raymarching are always a godsend. 

# How to run ProcSDF?

Until we release this as a binary - the easiest way to run this is installing Visual Studio 2019 (2022 might work too, but all developers currently use 2019), the C++ development thingie - Visual Studio Desktop C++ development pack (or anything similar, since C++ is no longer a default language since at least Visual Studio 19), and open the `ProcSDF.sln` file in VS 2019 before running the application.

# Tell me more about ProcSDF

- The GUI's made in the friendly neighbourhood GUI library - **ImGUI**. Check it out [here](https://github.com/ocornut/imgui).

- The pipeline and rendering stuff is handled in **OpenGL**.

- Features :
    - Primitives, Operations on them, translations and rotations.
    - Adding your own SDFs
    - Materials (coming soon)
    - and we'll figure the rest of the stuff out as we go.

- The name doesn't have much thought to it. Though it does have a nice ring to it. We were working on making a procedural node based 3D edition software that uses SDFs and so, comes the name **ProcSDF**
