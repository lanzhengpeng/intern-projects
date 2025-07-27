### 【0948】create thru holes 创建贯穿孔

#### 代码

```cpp
    static void create_thru_holes(void)  
    {  
        char  
            dia[20] = { "1.0" };  
        double  
            loc[3],  
            t_loc[3],  
            t_dir[3],  
            z_dir[3];  
        tag_t  
            face,  
            hole,  
            thru;  
        while (((face = select_point_on_face("Select face for hole", loc))  
                != NULL_TAG)  
            && ((thru = select_a_face("Select thru face")) != NULL_TAG)  
            && (prompt_hole_dia(dia)))  
        {  
            ask_face_dir(loc, face, z_dir);  
            UF_CALL(UF_MODL_create_simple_hole(loc, z_dir, dia, "", "",  
                face, thru, &hole));  
        }  
    }

```

#### 代码解析

> 这段NX Open C++代码用于在NX中创建一个贯穿孔。主要功能包括：
>
> 1. 选择面：用户选择要创建孔的面(face)，并获取该面的位置坐标(loc)。
> 2. 选择贯穿面：用户选择一个贯穿面(thru face)。
> 3. 输入孔直径：通过提示框让用户输入孔的直径(dia)。
> 4. 获取面方向：获取所选面的法向方向(z_dir)。
> 5. 创建贯穿孔：使用NX Open C++ API中的UF_MODL_create_simple_hole函数，根据输入的位置、方向、直径等信息，在指定的面上创建一个贯穿孔。
> 6. 循环创建：通过while循环，可以让用户重复选择面、贯穿面和输入直径，从而创建多个贯穿孔。
>
> 该代码实现了在NX中通过简单的交互，快速创建多个贯穿孔的功能。
>
