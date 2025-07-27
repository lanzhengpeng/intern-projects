### 【1480】is object wave linked 对象是否为波形关联

#### 代码

```cpp
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static logical is_sketch_geometry(tag_t object)  
    {  
        logical  
            is_sketch_geom = FALSE;  
        int  
            ii,  
            n;  
        tag_t  
            *geoms,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        while (!is_sketch_geom &&  
               ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG))  
        {  
            UF_CALL(UF_SKET_ask_geoms_of_sketch(sketch, &n, &geoms));  
            for (ii = 0; ii < n; ii++)  
                if (geoms[ii] == object)  
                {  
                    is_sketch_geom = TRUE;  
                    break;  
                }  
            if (n > 0) UF_free(geoms);  
        }  
        return is_sketch_geom;  
    }  
    static logical is_object_wave_linked(tag_t object)  
    {  
        logical  
            is_so,  
            is_wave = FALSE;  
        tag_t  
            feat = NULL_TAG;  
        char  
            *string;  
        if (!UF_MODL_ask_object_feat(object, &feat) && (feat != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feat, &string));  
            printf("%d = %s\n", feat, string);  
            if (strstr(string, "LINK") == string) is_wave = TRUE;  
            UF_free(string);  
        }  
        else  
        {  
            UF_CALL(UF_SO_is_so(object, &is_so));  
            if (is_so && is_sketch_geometry(object)) is_wave = TRUE;  
        }  
        return is_wave;  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. ask_next_sketch函数：遍历当前显示部件中的所有草图，并返回下一个草图的对象标签。
> 2. is_sketch_geometry函数：判断给定的对象是否属于某个草图中的几何体。该函数遍历当前显示部件中的所有草图，并检查草图中的几何体是否包含该对象。
> 3. is_object_wave_linked函数：判断给定的对象是否与Wave关联。该函数首先尝试获取对象所属的特征，并判断特征类型是否以"LINK"开头。如果对象不是特征，则判断是否是Sketch中的几何体，如果是则认为与Wave关联。
> 4. 整体上，该代码片段实现了在NX模型中判断对象是否属于草图，以及是否与Wave关联的功能。这些功能在二次开发中可能用于检查模型的某些约束条件，例如在生成或修改特征时需要满足的规则。
>
