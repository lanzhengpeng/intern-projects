### 【0369】ask vertices normals 获取面法线和顶点信息

#### 代码

```cpp
    /*HEAD ASK_VERTICES_NORMALS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，翻译如下：

注意：UF_print_syslog是V18版本新增的。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
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
        tag_t  
            sol_tag = NULL_TAG,  
            facet_tag = NULL_TAG;  
        double  
            (* facet_vertices)[3],  
            (* facet_normals)[3];  
        char  
            verts_char[3],  
            facets_char[4],  
            first_ln[132] = "Number of facets in part : ",  
            sec_ln[132] = "Maximun number of vertices is : ";  
        UF_FACET_parameters_t  
            parms;  
        UF_CALL(UF_FACET_ask_default_parameters(&parms));  
        parms.max_facet_edges = 3;  
        sol_tag = select_a_body("Select a body");  
        UF_CALL(UF_FACET_facet_solid(sol_tag, &parms, &facet_tag));  
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
            printf("Facet ID is : %d and Num of vertices is : %d\n",  
                    facet_id, verts_in_facet);  
            UF_CALL(UF_FACET_ask_normals_of_facet(facet_tag, facet_id,  
                    &norms_in_facet, (double (*)[3])facet_normals));  
            printf("Facet ID is : %d and Num of normals is : %d\n",  
                    facet_id, norms_in_facet);  
            for(ii = 0; ii < 3 ; ii ++)  
            {  
                printf("\t+=+point cordinate for vertice[%d] +=+=+\n", ii);  
                for(jj = 0; jj < 3 ; jj ++)  
                {  
                    printf("\t\tvert coordinate[%d] : %f\n",  
                                    jj, facet_vertices[ii][jj]);  
                    printf("\t\tnorm coordinate[%d] : %f\n",  
                                    jj,facet_normals[ii][jj]);  
                }  
            }  
            printf("================================================\n");  
        }  
        free(facet_vertices);  
        free(facet_normals);  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 错误报告函数：定义了report_error函数，用于打印错误信息，包括错误代码、文件名、行号和调用函数名。
> 2. 选择体函数：定义了mask_for_bodies和select_a_body函数，用于在选择对话框中过滤出实体类型，并让用户选择一个实体。
> 3. 处理实体：do_it函数中，首先调用UF_FACET_facet_solid将实体划分成面片。然后循环遍历每个面片，获取面片的顶点坐标和法向量，并打印出来。
> 4. 用户函数：ufusr函数是NX二次开发的入口函数，初始化NX后调用do_it函数，最后终止NX。
> 5. 卸载函数：ufusr_ask_unload函数用于在卸载时立即释放资源。
>
> 该代码的目的是演示如何使用NX的Facet模块来获取实体的顶点坐标和法向量。
>
