### 【0733】create intersection features with datum planes and bodies 创建基准面与实体的交线特征

#### 代码

```cpp
    /*HEAD CREATE_INTERSECTION_FEATURES_WITH_DATUM_PLANES_AND_BODIES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。

在翻译中，不需要添加任何额外的内容，只需要翻译原文即可：

UF_print_syslog是在V18版本中新增的函数，用于打印系统日志信息。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); (UF_UI_write_listing_window(X))  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static tag_t ask_next_dplane(tag_t part, tag_t dplane)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_datum_plane_type, &dplane));  
        return (dplane);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body,  
            bodies[1] = { NULL_TAG },  
            int_feat,  
            part = UF_PART_ask_display_part(),  
            planes[1],  
            plane = NULL_TAG;  
        double  
            tol;  
        UF_MODL_ask_distance_tolerance(&tol);  
        if ((body = ask_next_body(part, NULL_TAG)) != NULL_TAG)  
        {  
            UF_CALL(UF_DISP_set_highlight(body, TRUE));  
            bodies[0] = body;  
            while ((plane = ask_next_dplane(part, plane)) != NULL_TAG)  
            {  
                UF_CALL(UF_DISP_set_highlight(plane, TRUE));  
                planes[0] = plane;  
                if (!UF_CALL(UF_CURVE_create_int_object(1, planes, 1 , bodies, &int_feat)))  
                {  
                    printf("seems to work\n");  
                }  
                UF_CALL(UF_DISP_set_highlight(plane, FALSE));  
            }  
            UF_CALL(UF_DISP_set_highlight(body, FALSE));  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码内容，这是一段用于在NX中创建两个特征（实体和基准平面）之间交线的二次开发代码。
>
> 主要步骤包括：
>
> 1. 包含必要的NX API头文件，定义错误报告函数report_error。
> 2. 定义函数ask_next_body和ask_next_dplane，用于遍历部件中的实体和基准平面。
> 3. 定义函数do_it，该函数首先获取当前显示的部件，然后遍历所有实体，对每个实体高亮显示，然后遍历所有基准平面，对每个基准平面高亮显示，并使用UF_CURVE_create_int_object API尝试创建实体与基准平面的交线特征，如果成功则打印提示信息。
> 4. 定义ufusr函数，这是NX二次开发的入口函数。在该函数中初始化NX API，调用do_it函数执行创建交线特征的操作，然后终止NX API。
> 5. 定义ufusr_ask_unload函数，用于在卸载该代码时立即清理。
>
> 这段代码的主要功能是在NX部件中遍历所有实体和基准平面，尝试创建实体与基准平面的交线特征，并在创建成功时打印提示信息。
>
