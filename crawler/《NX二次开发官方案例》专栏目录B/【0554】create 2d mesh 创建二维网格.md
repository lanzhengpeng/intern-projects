### 【0554】create 2d mesh 创建二维网格

#### 代码

```cpp
    /*HEAD CREATE_2D_MESH CCC UFUN */  
    /* This program requires a face named "face1" 译:这个程序需要一个名为“face1”的面部。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据上下文，我理解了您的请求。UF_print_syslog 是 V18 中新增的函数，用于打印系统日志。您希望得到的是关于该函数的翻译，而不是其他无关的内容。以下是对 UF_print_syslog 的翻译：

UF_print_syslog 是 V18 中新增的函数，用于打印系统日志。 */  
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
    static void do_it(void)  
    {  
        tag_t                             mesh_id = NULL_TAG ;  
        tag_t                             obj_id, scenario_tag;  
        UF_SF_sfqm_mesh_data_t            mesh_data ;  
        UF_SF_sfqm_param_data_t           mesh_param_data = {0.02,0.0001,0.01,0.001,3};  
        mesh_data.elem_size         = .1 ;  
        mesh_data.mesh_param_data_p = &mesh_param_data ;  
        mesh_data.elem_type         = UF_SF_SFQM_TRIA3 ;  
        obj_id = NULL_TAG ;  
        UF_CALL( UF_SF_create_scenario( "scenario_test", &scenario_tag));  
        if ( UF_CALL( UF_OBJ_cycle_by_name("face1",&obj_id)) == 0)  
           UF_CALL( UF_SF_create_2d_mesh(1, &obj_id, 0, NULL, &mesh_data, &mesh_id)) ;  
    }  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发示例程序，其主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时输出错误信息。
> 2. 定义了一个do_it函数，用于创建一个2D网格。它首先获取名为"face1"的面，然后调用UF_SF_create_2d_mesh函数在该面上创建一个2D网格。网格的参数包括元素大小为0.1，元素类型为三角形单元，以及一些其他参数。
> 3. 定义了ufusr函数，这是NX二次开发程序的入口函数。在该函数中，先调用UF_initialize进行初始化，然后调用do_it函数创建网格，最后调用UF_terminate终止程序。
> 4. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发程序可以被立即卸载。
>
> 总的来说，这是一个简单的NX二次开发示例，展示了如何在NX中通过C++ API创建一个2D网格。
>
