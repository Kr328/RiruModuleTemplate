package com.github.kr328.plugin.riru.utils;

import java.io.File;

public class PathUtils {
    public static String toLocalSeparator(String p) {
        return p.replace('/' , File.separatorChar).replace('\\' ,File.separatorChar);
    }
}
