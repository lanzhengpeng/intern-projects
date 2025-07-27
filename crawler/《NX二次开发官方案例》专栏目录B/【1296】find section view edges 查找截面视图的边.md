### 【1296】find section view edges 查找截面视图的边

#### 代码

```cpp
    /*HEAD FIND_SECTION_VIEW_EDGES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog在V18版本中是一个新增的功能。 */  
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
    static void report_tag_type_and_subtype(char *name, tag_t object)  
    {  
        int  
            subtype,  
            type;  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("%s = %d, type = %d, subtype = %d\n", name,object,type,subtype);  
        }  
        else printf("%s = %d (NULL_TAG)\n", name, object);  
    }  
    static void do_it(void)  
    {  
        logical  
            is_a_sxview;  
        int  
            ii,  
            jj,  
            n_edges,  
            n_solids;  
        tag_t  
            d_tag,  
            *sxedges,  
            sxline,  
            *sxsolids,  
            v_tag;  
        char  
            view[UF_OBJ_NAME_LEN + 1] = { "" },  
            drawing[UF_OBJ_NAME_LEN + 1];  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_DRAW_ask_current_drawing(&d_tag));  
        UF_CALL(UF_OBJ_ask_name(d_tag, drawing));  
        while (!UF_CALL(uc6499(drawing, view)) && (strcmp(view, "")))  
        {  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view, &v_tag));  
            UF_CALL(UF_DRAW_is_sxview(v_tag, &is_a_sxview));  
            if (is_a_sxview)  
            {  
                UF_CALL(UF_DRAW_ask_sxline_of_sxview(v_tag, &sxline));  
                UF_CALL(UF_DRAW_ask_sxsolids_of_sxview(v_tag, UF_DRAW_sxline_leg1,  
                    &n_solids, &sxsolids));  
                for (ii = 0; ii < n_solids; ii++)  
                {  
                    UF_CALL(UF_DRAW_ask_sxedges_of_sxsolid(sxsolids[ii], &n_edges,  
                        &sxedges));  
                    for (jj = 0; jj < n_edges; jj++)  
                    {  
                        UF_CALL(UF_DISP_set_highlight(sxedges[jj], TRUE));  
                        sprintf(name, "sxedges[%d]", jj);  
                        report_tag_type_and_subtype(name, sxedges[jj]);  
                    }  
                    sprintf(name, "sxsolid[%d]", ii);  
                    uc1601(name, TRUE);  
                    for (jj = 0; jj < n_edges; jj++)  
                        UF_CALL(UF_DISP_set_highlight(sxedges[jj], FALSE));  
                    UF_free(sxedges);  
                }  
                UF_free(sxsolids);  
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

> 这段NX二次开发代码的主要功能是遍历当前图纸中的所有边视图，高亮显示每个边视图中的所有边线，并打印边线的对象类型和子类型。
>
> 主要步骤如下：
>
> 1. 获取当前图纸的tag。
> 2. 循环遍历图纸中的所有视图名称，直到找不到视图为止。
> 3. 检查视图是否为边视图，如果是，则获取边视图的tag和边线tag。
> 4. 获取边视图中的所有实体的tag，并遍历每个实体，获取每个实体的所有边线的tag。
> 5. 对每个边线进行高亮显示，并打印边线的对象类型和子类型。
> 6. 清除所有边线的高亮显示。
> 7. 重复步骤2-6，直到遍历完所有视图。
> 8. 释放内存。
> 9. 初始化和终止NX。
>
> 代码的关键点包括：
>
> 整体来看，代码结构清晰，逻辑严谨，符合NX二次开发的一般规范。
>
