### 【1955】report all scenario meshes 报告所有场景网格

#### 代码

```cpp
    /*HEAD REPORT_ALL_SCENARIO_MESHES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_sf.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。 */  
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
    static tag_t ask_next_of_type(tag_t part, int type, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object));  
        return (object);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_meshes;  
        tag_t  
            mesh = NULL_TAG,  
            mesh_recipe = NULL_TAG,  
            *meshes,  
            part = UF_PART_ask_display_part();  
        while ((mesh = ask_next_of_type(part, UF_sfem_mesh_type, mesh)) != NULL_TAG)  
            WRITE_D(mesh);  
        WRITE("\n");  
        while ((mesh_recipe = ask_next_of_type(part, UF_sfem_mesh_recipe_type,  
            mesh_recipe)) != NULL_TAG)  
        {  
            WRITE_D(mesh_recipe);  
            UF_CALL(UF_SF_find_mesh(mesh_recipe, UF_SF_DIMENSION_ANY, &n_meshes,  
                &meshes));  
            WRITE_D(n_meshes);  
            for (ii = 0; ii < n_meshes; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(meshes[ii]);  
            }  
        }  
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

> 这段代码是用于NX二次开发的UF程序，其主要功能是遍历当前显示部件中的所有网格对象，并报告网格对象及其关联的网格配方信息。具体代码实现如下：
>
> 1. 包含了必要的NX API头文件，如uf.h、uf_ui.h等。
> 2. 定义了错误报告函数report_error，用于输出错误信息和调用堆栈。
> 3. 定义了ask_next_of_type函数，用于遍历部件中指定类型的对象。
> 4. 定义了WRITE和WRITE_D宏，用于在列表窗口输出信息。
> 5. 定义了write_integer_to_listing_window函数，用于在列表窗口输出整数信息。
> 6. do_it函数是主函数，首先遍历所有网格对象并输出其tag。
> 7. 接着遍历所有网格配方对象，并输出其tag、关联的网格数量及这些网格的tag。
> 8. ufusr函数是程序入口，初始化NX API后调用do_it函数，然后终止NX API。
> 9. ufusr_ask_unload函数指定了程序卸载模式为立即卸载。
>
> 总体来说，这段代码实现了遍历当前部件中的所有网格对象及其关联的网格配方信息，并输出相关信息到列表窗口，以便进行调试和查看。
>
