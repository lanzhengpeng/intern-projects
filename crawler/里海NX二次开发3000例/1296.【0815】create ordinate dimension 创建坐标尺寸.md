### 【0815】create ordinate dimension 创建坐标尺寸

#### 代码

```cpp
    /*HEAD CREATE_ORDINATE_DIMENSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
    #include <uf_curve.h>  
    #include <uf_layout.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t ask_tag_of_current_view( void )  
    {  
        char  
            lay_name[30],  
            view_name[30];  
        tag_t  
            view_tag;  
        UF_CALL(uc6466(lay_name, view_name));  
        UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag));  
        return view_tag;  
    }  
    static tag_t create_origin(tag_t obj_tag, tag_t vw_tag)  
    {  
        UF_DRF_object_t  
            orig;  
        tag_t  
            orig_tag;  
        orig.object_view_tag = vw_tag;  
        orig.object_tag = obj_tag;  
        orig.object_assoc_type = UF_DRF_end_point;  
        orig.object_assoc_modifier = UF_DRF_first_end_point;  
        orig.object2_tag = NULL_TAG;  
        orig.assoc_dwg_pos[0]  = 0;  
        orig.assoc_dwg_pos[1]  = 0;  
        UF_CALL(UF_DRF_create_ordorigin(&orig, 1, 1, 1, "MY_ORIGIN",&orig_tag));  
        return orig_tag;  
    }  
    static tag_t create_margin(tag_t obj_tag, tag_t orig_tag, tag_t vw_tag)  
    {  
        UF_DRF_object_t  
            margin;  
        tag_t  
            marg_tag;  
        double  
            xy_pt[3],  
            xy_dir[2];  //changed this from 3 to 2 based on the docs.  
        margin.object_view_tag = vw_tag;  
        margin.object_tag = obj_tag;  
        margin.object_assoc_type = UF_DRF_end_point;  
        margin.object_assoc_modifier = UF_DRF_first_end_point;  
        margin.object2_tag = NULL_TAG;  
        margin.assoc_dwg_pos[0]  = 0;  
        margin.assoc_dwg_pos[1]  = 0;  
        UF_CALL(UF_DRF_create_ordmargin(1, orig_tag, &margin, xy_pt, xy_dir,  
            -2, &marg_tag));  //changed the margin type from horizontal 2, to vertical 1.  
        return marg_tag;  //corrected to return the margin tag instead of the origin tag.  
    }  
    static void create_ord_dim(tag_t obj_tag, tag_t marg_tag, tag_t vw_tag)  
    {  
        UF_DRF_object_t  
            ord_dim;  
        UF_DRF_text_t  
            marg_text = { NULL, 0 , NULL};  
        tag_t  
            dim_tag;  
        double  
            dim_orig[3] = {0,0,0};  
        ord_dim.object_view_tag = vw_tag;  
        ord_dim.object_tag = obj_tag;  
        ord_dim.object_assoc_type = UF_DRF_end_point;  
        ord_dim.object_assoc_modifier = UF_DRF_first_end_point;  
        ord_dim.object2_tag = NULL_TAG;  
        ord_dim.assoc_dwg_pos[0]  = 0;  
        ord_dim.assoc_dwg_pos[1]  = 0;  
        UF_CALL(UF_DRF_create_orddimension(marg_tag, 2,&ord_dim,45.0,1.0,  
                                           &marg_text, 2, dim_orig, &dim_tag));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            vw_tag,  
            ord_org_tag,  
            ord_marg_tag,  
            hor_line;  
        UF_CURVE_line_t  
            struct_hor_line = {{5,5,0},{8,5,0}};  
        UF_CALL(UF_CURVE_create_line(&struct_hor_line, &hor_line));  
        vw_tag = ask_tag_of_current_view();  
        ord_org_tag = create_origin(hor_line, vw_tag);  
        ord_marg_tag = create_margin(hor_line, ord_org_tag, vw_tag);  
        create_ord_dim(hor_line, ord_marg_tag, vw_tag);  
    }  
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

> 这段代码是NX二次开发代码，主要用于创建线性尺寸。下面是代码的详细介绍：
>
> 1. 引入了必要的NX Open API头文件。
> 2. 定义了错误处理宏UF_CALL，用于捕获函数调用错误并打印错误信息。
> 3. ask_tag_of_current_view函数获取当前视图的tag。
> 4. create_origin函数创建线性尺寸的原点。
> 5. create_margin函数创建线性尺寸的边距。
> 6. create_ord_dim函数创建线性尺寸。
> 7. do_it函数创建一个水平线，然后调用以上函数创建线性尺寸。
> 8. ufusr函数是NX二次开发的入口函数，调用do_it函数实现线性尺寸的创建。
> 9. ufusr_ask_unload函数用于卸载NX Open应用，这里设置为立即卸载。
>
> 总体来说，这段代码实现了线性尺寸的创建，包括原点、边距和尺寸线的创建。通过NX Open API函数的调用，实现了线性尺寸的自动绘制。
>
