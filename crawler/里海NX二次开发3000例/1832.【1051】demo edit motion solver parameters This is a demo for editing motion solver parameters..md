### 【1051】demo edit motion solver parameters This is a demo for editing motion solver parameters.

#### 代码

```cpp
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_motion.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X)  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    static void report_solver_parameters_structure_values(UF_MOTION_solver_parameters_t *solver_params)  
    {  
        switch (solver_params->solver)  
        {  
            case UF_MOTION_unknown_solver:  
                ECHO("solver_params->solver = UF_MOTION_unknown_solver\n");  
                break;  
            case UF_MOTION_kinematic_solver:  
                ECHO("solver_params->solver = UF_MOTION_kinematic_solver\n");  
                break;  
            case UF_MOTION_static_dynamic_solver:  
                ECHO("solver_params->solver = static_dynamic_solver\n");  
                break;  
            default:  
                ECHO("solver_params->solver = %d\n", solver_params->solver);  
                break;  
        }  
        WRITE_F(solver_params->max_step_size);  
        WRITE_F(solver_params->max_solver_error);  
        WRITE_D(solver_params->max_integrator_iterations);  
        WRITE_D(solver_params->max_kinematics_iterations);  
        WRITE_D(solver_params->max_statics_iterations);  
        WRITE_D(solver_params->use_mass_properties);  
        ECHO("\n");  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        UF_MOTION_solver_parameters_t  
            solver_params,  
            solver_params2;  
        UF_CALL(UF_MOTION_initialize());  
        UF_CALL(UF_MOTION_init_articulation());  
        UF_CALL(UF_MOTION_init_solver_parameters_struct(&solver_params));  
        ECHO("Default values as set by UF_MOTION_init_solver_parameters_struct\n");  
        report_solver_parameters_structure_values(&solver_params);  
        UF_CALL(UF_MOTION_ask_solver_parameters(&solver_params));  
        ECHO("Values from UF_MOTION_ask_solver_parameters\n");  
        report_solver_parameters_structure_values(&solver_params);  
        solver_params.solver = 3 - solver_params.solver;  //toggles 1->2->1  
        solver_params.max_step_size = solver_params.max_step_size + 0.1;  
        solver_params.max_solver_error = solver_params.max_solver_error + 0.1;  
        solver_params.max_integrator_iterations = solver_params.max_integrator_iterations + 10;  
        solver_params.max_kinematics_iterations = solver_params.max_kinematics_iterations + 10;  
        solver_params.max_statics_iterations = solver_params.max_statics_iterations + 10;  
        solver_params.use_mass_properties = abs(1 - solver_params.use_mass_properties); // toggles 0->1->0  
        set_undo_mark("Edit solver parameters");  
        UF_CALL(UF_MOTION_edit_solver_parameters(&solver_params));  
        ECHO("Values sent to UF_MOTION_edit_solver_parameters\n");  
        report_solver_parameters_structure_values(&solver_params);  
        UF_CALL(UF_MOTION_init_solver_parameters_struct(&solver_params2));  
        UF_CALL(UF_MOTION_ask_solver_parameters(&solver_params2));  
        ECHO("Values from UF_MOTION_ask_solver_parameters\n");  
        report_solver_parameters_structure_values(&solver_params2);  
        if (solver_params.solver != solver_params2.solver)  
            ECHO("! Failed to set solver !\n");  
        if (solver_params.max_step_size != solver_params2.max_step_size)  
            ECHO("! Failed to set max_step_size !\n");  // See PR 2187006  
        if (solver_params.max_solver_error != solver_params2.max_solver_error)  
            ECHO("! Failed to set max_solver_error !\n");  // See PR 2187006  
        if (solver_params.max_integrator_iterations != solver_params2.max_integrator_iterations)  
            ECHO("! Failed to set max_integrator_iterations !\n");  
        if (solver_params.max_kinematics_iterations != solver_params2.max_kinematics_iterations)  
            ECHO("! Failed to set max_kinematics_iterations !\n");  // See PR 2187006  
        if (solver_params.max_statics_iterations != solver_params2.max_statics_iterations)  
            ECHO("! Failed to set max_statics_iterations !\n");  
        if (solver_params.use_mass_properties != solver_params2.use_mass_properties)  
            ECHO("! Failed to set max_statics_iterations !\n");  // See PR 2187006  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码内容，这是一段基于NX Open API的UF_motion模块的二次开发示例。
>
> 主要功能包括：
>
> 1. 定义了一个宏函数ECHO，用于在窗口中输出信息。
> 2. 定义了一个宏函数UF_CALL，用于调用UF函数，并捕获错误信息。
> 3. 定义了一个函数report_error，用于输出错误信息。
> 4. 定义了一个函数report_solver_parameters_structure_values，用于输出运动求解器参数结构体的值。
> 5. 定义了一个函数set_undo_mark，用于设置撤销标记。
> 6. 定义了一个函数do_it，用于初始化求解器参数，输出默认值，修改参数，并输出修改后的值。同时检查修改是否成功。
> 7. 定义了一个函数ufusr，是二次开发的入口函数，调用do_it函数。
> 8. 定义了一个函数ufusr_ask_unload，用于卸载二次开发模块。
>
> 该代码通过调用UF_motion模块的相关函数，实现了对运动求解器参数的初始化、查询、修改和撤销标记的设置等操作，并通过日志输出和错误检查来验证操作结果。
>
