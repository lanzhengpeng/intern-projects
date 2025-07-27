### 【0988】create wrap geometry 创建包裹几何

#### 代码

```cpp
    /*HEAD CREATE_WRAP_GEOMETRY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <uf_obj.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            wrap_feat,  
            wrap_solid;  
        UF_MODL_wrap_geom_t  
            wrap_data;  
        wrap_data.close_gap = UF_WRAP_GEOM_CLOSE_BEVELED;  
        UF_MODL_ask_distance_tolerance(&wrap_data.dist_tol);  
        wrap_data.add_offset = "0.5";  
        wrap_data.split_offset = "0";  
        wrap_data.num_splits = 0;  
        wrap_data.splits = NULL;  
        while ((wrap_data.num_geoms = select_objects("Wrap Geometry",  
            &wrap_data.geometry)) > 0)  
        {  
            set_undo_mark("Wrap Geometry");  
            UF_CALL(UF_MODL_create_wrap_geometry(&wrap_data, &wrap_feat));  
            UF_free(wrap_data.geometry);  
            UF_CALL(UF_MODL_ask_feat_body(wrap_feat, &wrap_solid));  
            UF_CALL(UF_OBJ_set_color(wrap_solid, UF_OBJ_YELLOW));  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于打印出错误信息。
> 2. 定义了选择对象函数select_objects，用于弹出选择对话框，让用户选择要包裹几何体的对象。
> 3. 定义了设置撤销标记函数set_undo_mark，用于在执行操作前设置撤销标记。
> 4. 实现了主要功能函数do_it，其中包含以下步骤：不断循环，直到用户选择0个对象结束设置撤销标记调用UF_MODL_create_wrap_geometry函数创建包裹几何体特征清理内存获取包裹几何体特征生成的实体设置实体的颜色为黄色
> 5. 不断循环，直到用户选择0个对象结束
> 6. 设置撤销标记
> 7. 调用UF_MODL_create_wrap_geometry函数创建包裹几何体特征
> 8. 清理内存
> 9. 获取包裹几何体特征生成的实体
> 10. 设置实体的颜色为黄色
> 11. 实现了ufusr函数，用于初始化并执行do_it函数，然后终止NX。
> 12. 实现了ufusr_ask_unload函数，用于立即卸载用户自定义函数。
>
> 整体而言，这段代码通过封装选择、撤销、创建特征等函数，实现了一个简单的包裹几何体功能，并通过设置颜色进行可视化展示。
>
