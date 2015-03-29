#include "hbcidialog.h"
#include "ui_hbcidialog.h"

HBCIDialog::HBCIDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::HBCIDialog)
{
  ui->setupUi(this);
  ui->banksCb->addItem(QIcon(":/banks/banks/hyve.png"), "HypoVereinsbank (HVB)", QString("https://hbci-01.hypovereinsbank.de/bank/hbci"));
  ui->banksCb->addItem(QIcon(":/banks/banks/postbank.png"), "Postbank", QString("https://hbci.postbank.de/banking/hbci.do"));
  ui->banksCb->addItem(QIcon(":/banks/banks/sparkasse.png"), "Sparkasse", QString("https://hbci-pintan-XX.s-hbci.de/PinTanServlet"));
  ui->banksCb->addItem(QIcon(":/banks/banks/vobaraiba.png"), "Volksbank Raiffeisenbank", QString("https://hbci-pintan.gad.de/cgi-bin/hbciservlet"));
  ui->banksCb->addItem(tr("Other"));

  ui->regioCb->addItem("Baden-Württemberg", QString("BW"));
  ui->regioCb->addItem("Bayern", QString("BY"));
  ui->regioCb->addItem("Berlin", QString("BE"));
  ui->regioCb->addItem("Brandenburg", QString("BB"));
  ui->regioCb->addItem("Bremen", QString("HB"));
  ui->regioCb->addItem("Hamburg", QString("HH"));
  ui->regioCb->addItem("Hessen", QString("HE"));
  ui->regioCb->addItem("Mecklenburg-Vorpommern", QString("MV"));
  ui->regioCb->addItem("Niedersachsen", QString("NI"));
  ui->regioCb->addItem("Nordrhein-Westfalen", QString("NW"));
  ui->regioCb->addItem("Rheinland-Pfalz", QString("RP"));
  ui->regioCb->addItem("Saarland", QString("SL"));
  ui->regioCb->addItem("Sachsen", QString("SN"));
  ui->regioCb->addItem("Sachsen-Anhalt", QString("ST"));
  ui->regioCb->addItem("Schleswig-Holstein", QString("SH"));
  ui->regioCb->addItem("Thüringen", QString("TH"));

  connect(ui->banksCb, SIGNAL(currentIndexChanged(int)), this, SLOT(setHBCIURL()));
  connect(ui->regioCb, SIGNAL(currentIndexChanged(int)), this, SLOT(setHBCIURL()));
  setHBCIURL();
}

HBCIDialog::~HBCIDialog()
{
  delete ui;
}

QString HBCIDialog::getURL() const
{
  return ui->hbciLe->text();
}

QString HBCIDialog::getBLZ() const
{
  return ui->blzLe->text();
}

QString HBCIDialog::getUserID() const
{
  return ui->userIDLe->text();
}

QString HBCIDialog::getPin() const
{
  return ui->pinLe->text();
}

void HBCIDialog::setHBCIURL()
{
  int bank_idx = ui->banksCb->currentIndex();
  QString url = ui->banksCb->currentData().toString();

  if (bank_idx == 4)
    url = "";
  else if (bank_idx == 2)
    url = url.replace("XX", ui->regioCb->currentData().toString().toLower());

  ui->hbciLe->setText(url);
  ui->hbciLe->setReadOnly(bank_idx != 4);
}
