#include "stdafx.h"
#include "auditable.h"
#include <ctime>

/**
 * Auditable constructor: just initializes time fields.
 */
Auditable::Auditable() {
	time(&m_date_created);
	time(&m_date_modified);
}