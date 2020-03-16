#include "extensions/filters/listener/tls_inspector/tls_inspector_config_factory.h"

#include <string>

#include "envoy/extensions/filters/listener/tls_inspector/v3/tls_inspector.pb.h"
#include "envoy/extensions/filters/listener/tls_inspector/v3/tls_inspector.pb.validate.h"
#include "envoy/registry/registry.h"

#include "extensions/filters/listener/tls_inspector/tls_inspector.h"
#include "extensions/filters/listener/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace ListenerFilters {
namespace TlsInspector {

Network::ListenerFilterFactoryCb TlsInspectorConfigFactory::createFilterFactoryFromProto(
    const Protobuf::Message& message, Server::Configuration::ListenerFactoryContext& context) {
  auto proto_config = MessageUtil::downcastAndValidate<
      const envoy::extensions::filters::listener::tls_inspector::v3::TlsInspector&>(
      message, context.messageValidationVisitor());
  ConfigSharedPtr config(new Config(context.scope(), proto_config));
  return [config](Network::ListenerFilterManager& filter_manager) -> void {
    filter_manager.addAcceptFilter(std::make_unique<Filter>(config));
  };
}

ProtobufTypes::MessagePtr TlsInspectorConfigFactory::createEmptyConfigProto() {
  return std::make_unique<envoy::extensions::filters::listener::tls_inspector::v3::TlsInspector>();
}

/**
 * Static registration for the TLS inspector filter. @see RegisterFactory.
 */
REGISTER_FACTORY(TlsInspectorConfigFactory,
                 Server::Configuration::NamedListenerFilterConfigFactory){
    "envoy.listener.tls_inspector"};

} // namespace TlsInspector
} // namespace ListenerFilters
} // namespace Extensions
} // namespace Envoy
