Tiny Graphic Engine

Platforms: Windows, Android

Aims: just for fun and my self-learning

Current status: Render system is being developed

How to use(refresher for me): after building you will get a dynamic library which you should load in your project. Also you need to include TGE.h header which itself will load all needed headers for the engine. In the main function (like WinMain on Windows or android_main on Android) you need to obtain pointer to ICore interface using GetEngine function. Then you need to add your functions or static methods for init, free, process, render using pointer to ICore and its method AddFunction. Then you can invoke InitializeEngine method of interface ICore which will initialize engine and will start main loop. After all you should invoke FreeEngine function.

Using  method GetEngineSubsystem of ICore interface you can obtain pointers to engine subsystems like input, render, etc (interfaces are in TGE.h) and use their methods.
