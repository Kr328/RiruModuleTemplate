package com.github.kr328.plugin.riru.utils;

import java.io.File;
import java.util.Objects;
import java.util.function.Consumer;

public class PathUtils {
    public static String toLocalSeparator(String p) {
        return p.replace('/' , File.separatorChar).replace('\\' ,File.separatorChar);
    }

    public static String trimPathSeparator(String p) {
        return p.replaceAll("/+" ,"/");
    }

    public static synchronized String executableSuffix() {
        if ( exe == null ) {
            if ( System.getProperty("os.name").toLowerCase().contains("win") )
                exe = ".exe";
            else
                exe = "";
        }

        return exe;
    }

    private static String exe;
}
