### 【0427】ask magnitude force in absolute reference frame 查询绝对坐标系下力的大小

#### 代码

```cpp
    /*HEAD ASK_MAGNITUDE_FORCE_IN_ABSOLUTE_REFERENCE_FRAME CCC UFUN*/  
    /*This program requires a master model part and scenario part,   
    which contains a valid mechanism model. It has to be executed while  
    the scenario part is active in the UG/MOTION application.  
    Note: uf_motion.h and uf_motion_types.h were first released in V18.*/  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_motion.h>  
    #include <uf_motion_types.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog在V18中是新增的。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    /* This function returns the number of successful steps taken by the simulation 译:这个函数返回模拟成功的步骤数。 */  
    static int run_simulation( void )  
    {  
        const double total_time = 10.0;  
        int num_steps = 20;  
        UF_CALL( UF_MOTION_solve_model( total_time, &num_steps ) );  
        return num_steps;  
    }  
    static void do_it(void)  
    {  
        char   
            joint_name[]={"S002"}; /*use your own joint name here;*/  
        int steps = 0, num_results = 0, ii;  
        double  *results;  
        tag_t joint_tag = NULL_TAG;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UI_exit_listing_window());  
        UF_CALL(UF_MOTION_initialize());  
        steps = run_simulation();  
        printf("steps %d\n", steps);  
        UF_CALL( UF_OBJ_cycle_by_name( joint_name, &joint_tag ));   
        printf("joint_tag %d\n", joint_tag);  
        UF_CALL( UF_MOTION_ask_force_results( joint_tag,   
                                              UF_MOTION_magnitude,  
                                              UF_MOTION_absolute,  
                                              &num_results,  
                                              &results ) );  
        printf("number of results %d\n", num_results);  
        for (ii=0; ii < num_results; ii++)  
        {   
            printf("step %d, force results %f\n", ii, results[ii]);  
        }  
            if (num_results) UF_free(results);  
        UF_CALL(UF_MOTION_terminate());  
    }  
    /* 里海 */  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 初始化和错误报告：代码首先进行了NX的初始化，并定义了错误报告函数report_error，用于在调用NX API时捕捉错误并打印错误信息。
> 2. 模拟求解：函数run_simulation实现了对指定时间范围内进行指定步数模拟求解的功能。
> 3. 获取关节力结果：函数do_it实现了获取指定关节的力结果的功能。首先通过名称获取关节tag，然后调用UF_MOTION_ask_force_results函数获取指定关节在每一步的力大小，并打印输出。
> 4. 用户函数：定义了ufusr和ufusr_ask_unload函数，作为NX的二次开发入口。在ufusr函数中调用do_it函数来实现主要功能。
> 5. 资源释放：在获取完力结果后，释放了申请的内存。
> 6. 日志窗口：通过调用UF_UI_exit_listing_window函数，关闭了NX的日志窗口。
>
> 这段代码通过二次开发实现了获取NX模拟中指定关节的力结果的功能，展示了NX二次开发的基本流程。
>
