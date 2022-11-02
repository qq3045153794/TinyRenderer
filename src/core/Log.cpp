#include "core/Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

#include "assert.h"



std::shared_ptr<spdlog::logger> Log::logger;

void Log::init() {
  std::vector<std::shared_ptr<spdlog::sinks::stdout_color_sink_mt>> sinks;
  // 标准输出/标准错误输出 与颜色
  sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  // 设置日志格式
  // 规则连接：https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#customizing-format-using-set_pattern
  sinks[0]->set_pattern("[%^%4!l%$][%Y-%m-%d %H:%M:%S.%e]<%s:%#>: %v");
  sinks[0]->set_color(spdlog::level::trace, sinks[0]->cyan);
  sinks[0]->set_color(spdlog::level::debug, sinks[0]->bold);
  
  logger = std::make_shared<spdlog::logger>("logger", begin(sinks), end(sinks));

  // 全局使用需要注册
  spdlog::register_logger(logger);

  // 日志等级
  logger->set_level(spdlog::level::trace);

  logger->flush_on(spdlog::level::trace);
}

void Log::shut_down() { spdlog::shutdown(); }

