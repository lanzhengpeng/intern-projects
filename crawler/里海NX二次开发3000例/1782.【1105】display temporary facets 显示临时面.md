### 【1105】display temporary facets 显示临时面

#### 代码

```cpp
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void display_temporary_line(double start[3], double end[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
    }  
    static void display_temporary_facets(tag_t model)  
    {  
        int  
            ii,  
            jj,  
            n_facets,  
            n_verts;  
        double  
            (*verts)[3];  
        UF_CALL(UF_FACET_ask_max_facet_verts(model, &n_verts));  
        UF_CALL(allocate_memory(n_verts * 3 * sizeof(double), (void *)&verts));  
        UF_CALL(UF_FACET_ask_n_facets_in_model(model, &n_facets));  
        for (ii = 0; ii < n_facets; ii++)  
        {  
            UF_CALL(UF_FACET_ask_vertices_of_facet(model, ii, &n_verts, verts));  
            for (jj = 0; jj < (n_verts - 1); jj++)  
                display_temporary_line(verts[jj], verts[jj+1]);  
            display_temporary_line(verts[n_verts-1], verts[0]);  
        }  
        UF_free(verts);  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的C++函数，主要包括以下几个关键点：
>
> 1. allocate_memory函数：用于动态分配内存空间，通过UF_allocate_memory函数申请指定大小的内存，并将内存地址存储在where参数指向的指针中，返回值是申请内存的结果代码。
> 2. display_temporary_line函数：用于在NX中显示临时的线条。它接受线条的起点和终点坐标，以及线条属性，然后调用UF_DISP_display_temporary_line函数在NX中绘制线条。
> 3. display_temporary_facets函数：用于在NX中显示临时面。它首先获取模型中面的顶点数，然后动态申请内存以存储面的顶点坐标。接着，遍历模型中的所有面，获取每个面的顶点坐标，并调用display_temporary_line函数在NX中绘制每个面的边界线。最后，释放申请的内存。
> 4. 代码整体作用：这段代码用于在NX中动态显示模型的临时线条和面，为NX的二次开发提供了图形显示的功能支持。
>
