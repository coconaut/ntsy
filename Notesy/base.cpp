#include "stdafx.h"
#include "base.h"
#include <ctime>

/**
 * Auditable constructor: just initializes time fields.
 */
Base::Base() {
	time(&m_date_created);
	time(&m_date_modified);
}