### 【2229】report normals and vertices of faceted body 报告实体体的法线和顶点

#### 代码

```cpp
    /*HEAD REPORT_NORMALS_AND_VERTICES_OF_FACETED_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_facet.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的功能。

翻译如下：

注意：UF_print_syslog是V18版本新增的功能。 */  
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
    /* qq3123197280 */  
    static int mask_for_faceted_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_faceted_model_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_faceted_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select faceted body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faceted_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void print_text(char *text)  
    {  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(text);  
            }  
    }  
    static void query_facets(tag_t facet_tag)  
    {  
        int  
            irc,  
            ii,  
            jj,  
            facet_id = NULL_TAG,  
            verts_in_facet,  
            vertex_array_size,  
            norms_in_facet,  
            num_facets,  
            max_verts;  
        double  
            (* facet_vertices)[3],  
            (* facet_normals)[3];  
        char  
            msg[133],  
            verts_char[3],  
            facets_char[4],  
            first_ln[132] = "Number of facets in part : ",  
            sec_ln[132] = "Maximun number of vertices is : ";  
        UF_FACET_parameters_t  
            parms;  
        UF_CALL(UF_FACET_ask_default_parameters(&parms));  
         parms.max_facet_edges = 3;  
        UF_CALL(UF_FACET_ask_n_facets_in_model(facet_tag, &num_facets));  
        UF_CALL(UF_FACET_ask_max_facet_verts(facet_tag, &max_verts));  
        vertex_array_size = 3*max_verts*sizeof(double);  
        facet_vertices = (double(*)[3])malloc(vertex_array_size);  
        facet_normals = (double(*)[3])malloc(vertex_array_size);  
        sprintf(verts_char,"%d", max_verts);  
        sprintf(facets_char,"%d", num_facets);  
        strcat(first_ln,facets_char);  
        strcat(first_ln,"\n");  
        strcat(first_ln,sec_ln);  
        strcat(first_ln,verts_char);  
        uc1601(first_ln, 1);  
        UF_CALL(uc4400(0,"",0));  
        FTN(uf4401)(&irc);  
        while (!UF_CALL(UF_FACET_cycle_facets(facet_tag, &facet_id)) &&  
            (facet_id != UF_FACET_NULL_FACET_ID))  
        {  
            verts_in_facet = 0;  
            norms_in_facet = 0;  
            UF_CALL(UF_FACET_ask_vertices_of_facet(facet_tag, facet_id,  
                    &verts_in_facet, (double (*)[3])facet_vertices));  
            sprintf( msg, "Facet ID is : %d and Num of vertices is : %d\n",  
                    facet_id, verts_in_facet);  
            print_text(msg);  
            UF_CALL(UF_FACET_ask_normals_of_facet(facet_tag, facet_id,  
                    &norms_in_facet, (double (*)[3])facet_normals));  
            sprintf( msg, "Facet ID is : %d and Num of normals is : %d\n",  
                    facet_id, norms_in_facet);  
            print_text(msg);  
            for(ii = 0; ii < 3 ; ii ++)  
            {  
                sprintf( msg, "\t+=+point cordinate for vertice[%d] +=+=+\n", ii);  
                for(jj = 0; jj < 3 ; jj ++)  
                {  
                    sprintf( msg, "\t\tvert coordinate[%d] : %f\n",  
                                    jj, facet_vertices[ii][jj]);  
                    print_text(msg);  
                    sprintf( msg, "\t\tnorm coordinate[%d] : %f\n",  
                                    jj,facet_normals[ii][jj]);  
                    print_text(msg);  
               }  
            }  
            print_text("================================================\n");  
        }  
        free(facet_vertices);  
        free(facet_normals);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            fbody;  
            fbody = select_a_faceted_body("Select faceted body");  
            query_facets(fbody);  
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

> 这段代码是一个NX Open C++应用程序，用于查询NX部件中的Facet Body的顶点和法线信息。
>
> 主要功能包括：
>
> 1. 错误报告：使用宏UF_CALL捕获并报告NX API调用错误。
> 2. Facet Body选择：使用UF_UI函数创建选择对话框，允许用户选择一个Facet Body。
> 3. Facet Body信息查询：使用UF_FACET函数查询Facet Body中的Facet数量、最大顶点数、每个Facet的顶点和法线信息。
> 4. 结果输出：将查询结果输出到日志窗口或系统日志。
> 5. 内存管理：动态分配内存以存储顶点和法线信息。
> 6. 初始化和终止：在ufusr函数中进行NX初始化和终止。
> 7. 卸载：ufusr_ask_unload函数实现立即卸载。
>
> 总体而言，这段代码通过NX API实现了选择和查询Facet Body几何信息的功能，并以可读性高的方式组织了代码结构。
>
