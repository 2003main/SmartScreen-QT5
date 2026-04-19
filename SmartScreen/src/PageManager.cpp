#include "PageManager.h"

PageManager::PageManager(QObject *parent)
    : QObject(parent) {}

void PageManager::switchTo(Page page)
{
    if (m_current == page) return;
    m_current = page;
    emit pageChanged(static_cast<int>(page));
}
