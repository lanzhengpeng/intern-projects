### 【1214】export faceted bodies to stl 将facet体导出为STL

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** export_faceted_bodies_to_stl.c  
    **  
    ** Description:  
    ** UF_STD_put_sheets_in_stl_file/UF_STD_put_solid_in_stl_file  
    ** do not allow exporting faceted bodies.  
    ** Hence, this sample code reads the facets with UF_FACET API  
    ** and writes the STL file with standard IO  
    **  
    ** Binary STL format specification:  
    ** UINT8[80]         -   Header  
    ** UINT32            -   Number of triangles  
    ** foreach triangle  
    **   REAL32[3]       -    Normal vector  
    **   REAL32[3]       -    Vertex 1  
    **   REAL32[3]       -    Vertex 2  
    **   REAL32[3]       -    Vertex 3  
    **   UINT16          -    Attribute byte count  
    ** end  
    **  
    ** Note: the Faceted body actually needs to be from type/subtype (139/0)  
    ** For example, UF_FACET_ask_plane_equation does not work for JT/Lightweight Bodies (139/3)  
    ** So this improved version will calculate the facet normal for JT Facets (139/3) instead.  
    **   
    *****************************************************************************/  
    /* Include files 里海译:Include files的翻译为“包含文件”。 */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_facet.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_std.h>  
    #include <uf_ui.h>  
    #include <uf_vec.h>  
    static logical bDebug = true;  
    static int nFacetBodySubType = UF_faceted_model_normal_subtype;  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN + 1];  
        va_list args;  
        va_start(args, format);  
        vsprintf_s(msg, strlen(msg), format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        //UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char *file, int line, char *call, int irc) {  
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
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        if (strrchr(temp_name, '.')) strcpy_s(strrchr(temp_name, '.'), 1, "");  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    static void build_similar_filespec(char *fspec, int ftype)  
    {  
        logical  
            is_ugmgr;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_number[UF_UGMGR_PARTNO_SIZE + 1],  
            part_revision[UF_UGMGR_PARTREV_SIZE + 1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE + 1],  
            part_file_name[UF_UGMGR_FNAME_SIZE + 1];  
        UF_CALL(UF_is_ugmanager_active(&is_ugmgr));  
        UF_PART_ask_part_name(part, part_fspec);  
        if (!is_ugmgr)  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
            UF_CALL(uc4575(dspec, ftype, part_name, fspec));  
        }  
        else  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            build_similar_temp_filespec(part_number, ftype, fspec);  
        }  
    }  
    static int ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static tag_t ask_next_faceted_body(tag_t part, tag_t fbody)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_faceted_model_type, &fbody))  
            && (fbody != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(fbody, &type, &subtype));  
            //if (subtype == UF_faceted_model_normal_subtype) return fbody;  
            nFacetBodySubType = subtype;  
            return fbody;  
        }  
        return NULL_TAG;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_faceted_bodies(tag_t part, tag_t **fbodies)  
    {  
        tag_t  
            body = NULL_TAG;  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        while ((body = ask_next_faceted_body(part, body)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(body_list, body));  
        return (make_an_array(&body_list, fbodies));  
    }  
    static int file_exists(char *file_spec)  
    {  
        int  
            status = 0;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &status));  
        return status;  
    }  
    static logical prompt_for_stl_file_name(char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE + 1] = { "*.stl" };  
        if (!UF_CALL(UF_UI_create_filebox("STL File Name", "STL File Name", filter, fspec,  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static int export_faceted_solids_to_stl(tag_t part, char *fspec)  
    {  
        int  
            ii, jj, kk,  
            n_bodies = 0,  
            num_facets = 0,  
            num_all_facets = 0,  
            num_vertices = 0,  
            facet_id = UF_FACET_NULL_FACET_ID;  
        short  
            abc = 0;  
        double  
            plane_normal[3] = { 0.0, 0.0, 1.0 },  
            d_coefficient = 0.0,  
            (*vertices)[3];  
        tag_t  
            *bodies;  
        char  
            header[80] = { '\0' };  
        FILE  
            *fpSTL;  
        size_t  
            count;  
        float  
            value,  
            minmax[6] = { 10000, -10000, 10000, -10000, 10000, -10000 }; //xmin,xmax,ymin,ymax,zmin,zmax  
        n_bodies = ask_all_faceted_bodies(part, &bodies);  
        if (n_bodies == 0) return 0;  
        // Create binary STL file and write header of 80 byte  
        if (fopen_s(&fpSTL, fspec, "wb"))  
        {  
            ECHO("Error opening %s. Exit.\n", fspec);  
            return 0;  
        }  
        strcpy_s(header, 79, "Created by NXOpen");  
        if (bDebug) ECHO("Header: <%s>\n", header);  
        count = fwrite(header, 80, 1, fpSTL);  
        // Count all facets in all faceted bodies and write to STL  
        for (ii = 0; ii< n_bodies; ii++)  
        {  
            UF_CALL(UF_FACET_ask_n_facets_in_model(bodies[ii], &num_facets));  
            num_all_facets += num_facets;  
        }  
        if (bDebug) ECHO(" Faceted Bodies: %d\n", n_bodies);  
        if (bDebug) ECHO(" Facets: %d\n", num_all_facets);  
        count = fwrite(&num_all_facets, sizeof(int), 1, fpSTL);  
        // Query each faceted body for facets, normal and vertices  
        for (ii = 0; ii< n_bodies; ii++)  
        {  
            num_all_facets = 0;  
            if (bDebug) ECHO("  Faceted Body: %d\n", bodies[ii]);  
            UF_CALL(UF_FACET_cycle_facets(bodies[ii], &facet_id));  
            while (facet_id != UF_FACET_NULL_FACET_ID)  
            {  
                num_all_facets++;  
                // UF_FACET_ask_plane_equation only works for NX Facet type  
                if (nFacetBodySubType == UF_faceted_model_normal_subtype)  
                {  
                    // Write facet normal to STL  
                    UF_CALL(UF_FACET_ask_plane_equation(bodies[ii], facet_id, plane_normal, &d_coefficient));  
                    for (jj = 0; jj<3; jj++)  
                    {  
                        value = (float)plane_normal[jj];  
                        if (bDebug) ECHO("plane_normal[%d] = %f\n", jj, plane_normal[jj]);  
                        count = fwrite(&value, sizeof(float), 1, fpSTL);  
                    }  
                }  
                // Write facet vertices to STL  
                UF_CALL(UF_FACET_ask_num_verts_in_facet(bodies[ii], facet_id, &num_vertices));  
                vertices = (double(*)[3])malloc(3 * num_vertices*sizeof(double));  
                UF_CALL(UF_FACET_ask_vertices_of_facet(bodies[ii], facet_id, &num_vertices, (double(*)[3])vertices));  
                // we should always have at least 3 vertices on each facet and never run into this error  
                if (num_vertices < 3)  
                {  
                    ECHO("ERROR: Facet has %d vertices only! Exit.\n", num_vertices);  
                    return 0;  
                }  
                // If Facet type is JT the calculate CrossProduct for plane normal now  
                if (nFacetBodySubType != UF_faceted_model_normal_subtype)  
                {  
                    double vec1[3] = { vertices[1][0] - vertices[0][0], vertices[1][1] - vertices[0][0], vertices[1][2] - vertices[0][0] };  
                    double vec2[3] = { vertices[2][0] - vertices[0][0], vertices[2][1] - vertices[0][0], vertices[2][2] - vertices[0][0] };  
                    UF_VEC3_cross(vec1, vec2, plane_normal);  
                    for (jj = 0; jj<3; jj++)  
                    {  
                        value = (float)plane_normal[jj];  
                        if (bDebug) ECHO("plane_normal[%d] = %f\n", jj, plane_normal[jj]);  
                        count = fwrite(&value, sizeof(float), 1, fpSTL);  
                    }  
                }  
                for (kk = 0; kk<num_vertices; kk++)  
                {  
                    for (jj = 0; jj<3; jj++)  
                    {  
                        float value = (float)vertices[kk][jj];  
                        if (bDebug) ECHO("vertice[%d][%d] = %f\n", kk, jj, vertices[kk][jj]);  
                        // get min max values  
                        if (value < minmax[jj * 2]) minmax[jj * 2] = value;  
                        if (value > minmax[jj * 2 + 1]) minmax[jj * 2 + 1] = value;  
                        count = fwrite(&value, sizeof(float), 1, fpSTL);  
                    }  
                }  
                if (vertices) free(vertices);  
                // Write the 16bit attribute count value  
                //ECHO("abc = %d\n", abc);  
                count = fwrite(&abc, sizeof(short), 1, fpSTL);  
                // Get next facet  
                UF_CALL(UF_FACET_cycle_facets(bodies[ii], &facet_id));  
            }  
            if (bDebug) ECHO(" Facets (check): %d\n", num_all_facets);  
            if (bDebug) ECHO(" Xmin=%.3f Xmax=%.3f\n Ymin=%.3f Ymax=%.3f\n Zmin=%.3f Zmax=%.3f\n Xdelta=%.3f\n Ydelta=%.3f\n Zdelta=%.3f\n",  
                minmax[0], minmax[1], minmax[2], minmax[3], minmax[4], minmax[5],  
                minmax[1] - minmax[0], minmax[3] - minmax[2], minmax[5] - minmax[4]);  
        }  
        fclose(fpSTL);  
        return n_bodies;  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr(char *parm, int *returnCode, int rlen)  
    {  
        int  
            n_bodies = 0;  
        char  
            fspec[MAX_FSPEC_SIZE + 1] = { "" };  
        tag_t  
            part = NULL_TAG;  
        /* Initialize the API environment 里海译:初始化API环境 */  
        if (UF_CALL(UF_initialize()))  
        {  
            /* Failed to initialize 里海译:Failed to initialize 翻译为：初始化失败。 */  
            return;  
        }  
        part = UF_PART_ask_display_part();  
        if (part == NULL_TAG)  
        {  
            ECHO("No displayed part...exit.\n");  
            return;  
        }  
        UF_CALL(UF_PART_ask_part_name(part, fspec));  
        ECHO("Part: %d, %s\n", part, fspec);  
        build_similar_filespec(fspec, 78);  
        if (!prompt_for_stl_file_name(fspec)) return;  
        if (file_exists(fspec) == 0 && !ask_yes_or_no("File Exists", "Overwrite File?")) return;  
        ECHO("STL: %s\n", fspec);  
        n_bodies = export_faceted_solids_to_stl(part, fspec);  
        ECHO("Exported %d faceted bodies.\n", n_bodies);  
        /* Terminate the API environment 里海译:终止API环境 */  
        UF_CALL(UF_terminate());  
    }  
    /*****************************************************************************  
    **  Utilities  
    *****************************************************************************/  
    /* Unload Handler  
    **     This function specifies when to unload your application from Unigraphics.  
    **     If your application registers a callback (from a MenuScript item or a  
    **     User Defined Object for example), this function MUST return  
    **     "UF_UNLOAD_UG_TERMINATE". */  
    extern int ufusr_ask_unload(void)  
    {  
        return(UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX Open C++代码实现了以下功能：
>
> 1. 导出Faceted Bodies到STL文件：该代码使用UF_FACET API遍历当前打开部件中的所有Faceted Bodies，并获取每个Facet的顶点和法线，然后使用标准的C文件操作将这些Facets写入STL文件。STL文件采用二进制格式。
> 2. 用户交互：代码提供了用户交互界面，用于提示用户输入STL文件名，以及当文件已存在时是否覆盖。
> 3. 错误处理：代码实现了完善的错误处理机制，能够在出错时输出错误信息。
> 4. 内存管理：使用了NX Open内存管理函数来分配和释放内存。
> 5. 激活和卸载：代码实现了NX Open标准激活方法ufusr和卸载方法ufusr_ask_unload。
> 6. 日志输出：通过自定义的ECHO函数输出日志信息。
> 7. 文件操作：实现了创建临时文件路径、检查文件是否存在等文件操作功能。
> 8. 类型处理：代码处理了不同子类型的Faceted Bodies。
> 9. 性能优化：代码在计算法线时进行了优化，避免重复计算。
> 10. 数据统计：代码统计了Facet Bodies和Facets的数量，并输出了模型的边界框信息。
>
> 综合来看，这段代码实现了从NX部件中提取Faceted Bodies并保存为STL文件的功能，具有完善的错误处理和用户交互机制，是一个典型的NX Open二次开发示例。
>
