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
package org.secretflow.dataproxy.plugin.odps.producer;

import com.google.protobuf.Any;
import org.apache.arrow.flight.FlightDescriptor;
import org.apache.arrow.flight.FlightInfo;
import org.apache.arrow.flight.FlightProducer;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.secretflow.v1alpha1.kusciaapi.Domaindata;
import org.secretflow.v1alpha1.kusciaapi.Domaindatasource;
import org.secretflow.v1alpha1.kusciaapi.Flightdm;
import org.secretflow.v1alpha1.kusciaapi.Flightinner;

import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.Mockito.when;

/**
 * @author 月榭
 * @date 2025/03/05 16:23:34
 */
@ExtendWith(MockitoExtension.class)
public class OdpsFlightProducerTest {

    @InjectMocks
    private OdpsFlightProducer odpsFlightProducer;

    @Mock
    private FlightProducer.CallContext context;

    @Mock
    private FlightDescriptor descriptor;

    private final Domaindatasource.OdpsDataSourceInfo odpsDataSourceInfo =
            Domaindatasource.OdpsDataSourceInfo
                    .newBuilder()
                    .setAccessKeyId("accessKeyId")
                    .setAccessKeySecret("accessKeySecret")
                    .setProject("project")
                    .setEndpoint("endpoint")
                    .build();
    private final Domaindatasource.DataSourceInfo dataSourceInfo =
            Domaindatasource.DataSourceInfo.newBuilder().setOdps(odpsDataSourceInfo).build();

    private final Domaindatasource.DomainDataSource domainDataSource =
            Domaindatasource.DomainDataSource.newBuilder()
                    .setDatasourceId("datasourceId")
                    .setName("datasourceName")
                    .setType("odps")
                    .setInfo(dataSourceInfo)
                    .build();

    private final Domaindata.DomainData domainData =
            Domaindata.DomainData.newBuilder()
                    .setDatasourceId("datasourceId")
                    .setName("domainDataName")
                    .setRelativeUri("table_name_or_file_path")
                    .setDomaindataId("domainDataId")
                    .setType("table")
                    .build();

    private final Flightdm.CommandDomainDataQuery commandDomainDataQueryWithRaw =
            Flightdm.CommandDomainDataQuery.newBuilder()
                    .setContentType(Flightdm.ContentType.RAW)
                    .setPartitionSpec("partition_spec")
                    .build();

    @Test
    public void testGetProducerName() {
        String producerName = odpsFlightProducer.getProducerName();
        assertEquals("odps", producerName);
    }

    @Test
    public void testGetFlightInfoWithTableCommand() {
        Flightinner.CommandDataMeshQuery dataMeshQuery =
                Flightinner.CommandDataMeshQuery.newBuilder()
                        .setQuery(commandDomainDataQueryWithRaw)
                        .setDatasource(domainDataSource)
                        .setDomaindata(domainData)
                        .build();
        when(descriptor.getCommand()).thenReturn(Any.pack(dataMeshQuery).toByteArray());
        assertDoesNotThrow(() -> {
            FlightInfo flightInfo = odpsFlightProducer.getFlightInfo(context, descriptor);

            assertNotNull(flightInfo);
            assertFalse(flightInfo.getEndpoints().isEmpty());

            assertNotNull(flightInfo.getEndpoints().get(0).getLocations());
            assertFalse(flightInfo.getEndpoints().get(0).getLocations().isEmpty());

            assertNotNull(flightInfo.getEndpoints().get(0).getTicket());
            assertNotNull(flightInfo.getEndpoints().get(0).getTicket().getBytes());

        });
    }

    @Test
    public void testGetFlightInfoWithUnsupportedType() {
        when(descriptor.getCommand()).thenReturn("testCommand".getBytes(StandardCharsets.UTF_8));
        assertThrows(RuntimeException.class, () -> odpsFlightProducer.getFlightInfo(context, descriptor));
    }

}
