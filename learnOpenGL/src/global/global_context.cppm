export module global_context;
export import global_context.log_system;

import <memory>;

export class RuntimeGlobalContext{
public:
    static void StartSystem();
    static void ShutdowmSystem();
};
