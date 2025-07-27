### 【0234】ask geom data of mesh 询问网格的几何数据

#### 代码

```cpp
    /*HEAD ASK_GEOM_DATA_OF_MESH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    /* 里海 */  
    static int mask_for_meshes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_sfem_mesh_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_mesh(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a mesh", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_meshes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_mesh_recipe_of_mesh(tag_t mesh)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *meshes,  
            part = UF_ASSEM_ask_work_part(),  
            recipe = NULL_TAG;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part( part, UF_sfem_mesh_recipe_type,  
            &recipe)) && (recipe != NULL_TAG))  
        {  
            UF_CALL(UF_SF_find_mesh(recipe, UF_SF_DIMENSION_ANY, &n, &meshes));  
            for (ii = 0; ii < n; ii++)  
                if (meshes[ii] == mesh)  
                {  
                    UF_free(meshes);  
                    return recipe;  
                }  
            if (n > 0) UF_free(meshes);  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            mesh,  
            recipe;  
        tag_t  
            *geoms;  
        if ((mesh = select_a_mesh("Select mesh")) != NULL_TAG)  
        {  
            UF_CALL(UF_SF_ask_geom_data(mesh, UF_SF_GEOM_ANY, &n, &geoms));  
            ECHO("After UF_SF_ask_geom_data(mesh... n = %d\n", n);  
            if (n > 0) UF_free(geoms);  
            recipe = ask_mesh_recipe_of_mesh(mesh);  
            if (recipe != NULL_TAG)  
            {  
                UF_CALL(UF_SF_ask_geom_data(recipe, UF_SF_GEOM_ANY, &n, &geoms));  
                ECHO("After UF_SF_ask_geom_data(recipe... n = %d\n", n);  
                if (n > 0) UF_free(geoms);  
            }  
            else  
                ECHO("recipe == NULL_TAG\n");  
        }  
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

> 根据代码，这是一个用于获取NX中网格几何数据的二次开发示例代码。主要功能如下：
>
> 1. 定义了错误报告函数report_error，用于在调用UF函数失败时报告错误。
> 2. 定义了选择网格的函数select_a_mesh，使用UF_UI_select_with_single_dialog函数选择一个网格，并返回网格的tag。
> 3. 定义了获取网格对应网格配方的函数ask_mesh_recipe_of_mesh，通过循环遍历零件中的网格配方，找到包含指定网格的配方，并返回配方的tag。
> 4. 定义了do_it函数，首先选择一个网格，然后使用UF_SF_ask_geom_data函数获取网格的几何数据，并输出数据数量。接着获取网格对应的网格配方，并同样获取其几何数据并输出。
> 5. 定义了ufusr函数，首先初始化UF，然后调用do_it函数执行操作，最后终止UF。
> 6. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 综上，该代码实现了选择网格、获取网格及网格配方的几何数据，并进行了输出，以实现获取网格几何数据的目的。
>
