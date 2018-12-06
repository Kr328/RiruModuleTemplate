package com.github.kr328.plugin.riru.exts;

import com.github.kr328.plugin.riru.utils.PathUtils;

import java.util.HashMap;

public class MagiskExtension {
    private HashMap<String ,String> zipMap = new HashMap<>();

    public MagiskExtension map(String source ,String target) {
        zipMap.put(PathUtils.toLocalSeparator(source) ,target);
        return this;
    }

    public HashMap<String ,String> getZipMap() {
        return zipMap;
    }
}
