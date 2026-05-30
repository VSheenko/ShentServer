//
// Created by vovan on 02.04.2025.
//

#include "PqSettings.h"

#include <format>

std::string PqSettings::getConnString() {
	return std::format("hostaddr={} port={} dbname={} user={} password={}",
		hostaddr, port, dbname, user, password);
}
