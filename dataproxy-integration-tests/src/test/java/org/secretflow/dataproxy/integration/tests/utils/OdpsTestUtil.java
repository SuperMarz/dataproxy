/*
 * Copyright 2025 Ant Group Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.secretflow.dataproxy.integration.tests.utils;

import java.io.InputStream;
import java.util.Properties;

/**
 * @author yuexie
 * @date 2025/2/28 14:41
 **/
public class OdpsTestUtil {

    private static final Properties properties = new Properties();

    static {
        try (InputStream is = OdpsTestUtil.class.getResourceAsStream("/test-odps.conf")) {
            properties.load(is);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static String getOdpsProject() {
        return properties.getProperty("test.odps.project");
    }

    public static String getOdpsEndpoint() {
        return properties.getProperty("test.odps.endpoint");
    }

    public static String getAccessKeyId() {
        return properties.getProperty("test.odps.access.key.id");
    }

    public static String getAccessKeySecret() {
        return properties.getProperty("test.odps.access.key.secret");
    }

}
