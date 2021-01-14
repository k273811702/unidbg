#include <Hypervisor/Hypervisor.h>

#include <stdio.h>

#include <mach-o/dyld.h>
#include <mach-o/dyld_images.h>

static void hex(char *buf, void *ptr, size_t size) {
  const char *data = (const char *) ptr;
  int idx = 0;
  for(int i = 0; i < size; i++) {
    idx += sprintf(&buf[idx], "%02x", data[i] & 0xff);
  }
}

typedef struct vcpu_context {
  char _p00[8];
  uint64_t HV_REG_X0;
  uint64_t HV_REG_X1;
  uint64_t HV_REG_X2;
  uint64_t HV_REG_X3;
  uint64_t HV_REG_X4;
  uint64_t HV_REG_X5;
  uint64_t HV_REG_X6;
  uint64_t HV_REG_X7;
  uint64_t HV_REG_X8;
  uint64_t HV_REG_X9;
  uint64_t HV_REG_X10;
  uint64_t HV_REG_X11;
  uint64_t HV_REG_X12;
  uint64_t HV_REG_X13;
  uint64_t HV_REG_X14;
  uint64_t HV_REG_X15;
  uint64_t HV_REG_X16;
  uint64_t HV_REG_X17;
  uint64_t HV_REG_X18;
  uint64_t HV_REG_X19;
  uint64_t HV_REG_X20;
  uint64_t HV_REG_X21;
  uint64_t HV_REG_X22;
  uint64_t HV_REG_X23;
  uint64_t HV_REG_X24;
  uint64_t HV_REG_X25;
  uint64_t HV_REG_X26;
  uint64_t HV_REG_X27;
  uint64_t HV_REG_X28;
  uint64_t HV_REG_X29; // HV_REG_FP
  uint64_t HV_REG_X30; // HV_REG_LR
  uint64_t unknown0x100;
  uint64_t HV_REG_PC; // 0x108
  uint64_t HV_REG_CPSR; // 0x110

  char _p111[0x28];
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q0;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q1;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q2;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q3;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q4;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q5;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q6;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q7;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q8;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q9;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q10;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q11;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q12;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q13;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q14;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q15;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q16;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q17;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q18;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q19;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q20;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q21;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q22;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q23;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q24;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q25;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q26;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q27;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q28;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q29;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q30;
  hv_simd_fp_uchar16_t HV_SIMD_FP_REG_Q31;

  char _p0[0x10];
  uint64_t HV_SYS_REG_MDSCR_EL1;
  uint64_t HV_SYS_REG_TPIDR_EL1;
  uint64_t HV_SYS_REG_TPIDR_EL0;
  uint64_t HV_SYS_REG_TPIDRRO_EL0;
  uint64_t HV_SYS_REG_SP_EL0;
  uint64_t HV_SYS_REG_SP_EL1;
  uint64_t HV_SYS_REG_PAR_EL1;
  uint64_t HV_SYS_REG_CSSELR_EL1;
  uint64_t ext_reg;
  uint64_t unknown0x398;
  uint64_t HV_SYS_REG_TTBR0_EL1;
  uint64_t HV_SYS_REG_TTBR1_EL1;
  uint64_t HV_SYS_REG_TCR_EL1;
  uint64_t HV_SYS_REG_ELR_EL1;
  uint64_t HV_SYS_REG_FAR_EL1;
  uint64_t HV_SYS_REG_ESR_EL1;
  char _p002[0x10];
  uint64_t HV_SYS_REG_VBAR_EL1; // 0x3e0
  uint64_t HV_SYS_REG_CNTV_CVAL_EL0;
  uint64_t HV_SYS_REG_MAIR_EL1;
  uint64_t HV_SYS_REG_AMAIR_EL1;
  uint64_t HV_SYS_REG_SCTLR_EL1;
  uint64_t HV_SYS_REG_CPACR_EL1;
  uint64_t HV_SYS_REG_SPSR_EL1;
  uint64_t HV_SYS_REG_AFSR0_EL1;
  uint64_t HV_SYS_REG_AFSR1_EL1;
  uint64_t HV_SYS_REG_CONTEXTIDR_EL1;
  uint64_t HV_SYS_REG_CNTV_CTL_EL0; // 0x430
  char _p11[8];
  uint64_t HV_SYS_REG_CNTKCTL_EL1;
  uint64_t HV_SYS_REG_DBGBVR0_EL1;
  uint64_t HV_SYS_REG_DBGBCR0_EL1;
  uint64_t HV_SYS_REG_DBGBVR1_EL1;
  uint64_t HV_SYS_REG_DBGBCR1_EL1;
  uint64_t HV_SYS_REG_DBGBVR2_EL1;
  uint64_t HV_SYS_REG_DBGBCR2_EL1;
  uint64_t HV_SYS_REG_DBGBVR3_EL1;
  uint64_t HV_SYS_REG_DBGBCR3_EL1;
  uint64_t HV_SYS_REG_DBGBVR4_EL1;
  uint64_t HV_SYS_REG_DBGBCR4_EL1;
  uint64_t HV_SYS_REG_DBGBVR5_EL1;
  uint64_t HV_SYS_REG_DBGBCR5_EL1;
  uint64_t HV_SYS_REG_DBGBVR6_EL1;
  uint64_t HV_SYS_REG_DBGBCR6_EL1;
  uint64_t HV_SYS_REG_DBGBVR7_EL1;
  uint64_t HV_SYS_REG_DBGBCR7_EL1;
  uint64_t HV_SYS_REG_DBGBVR8_EL1;
  uint64_t HV_SYS_REG_DBGBCR8_EL1;
  uint64_t HV_SYS_REG_DBGBVR9_EL1;
  uint64_t HV_SYS_REG_DBGBCR9_EL1;
  uint64_t HV_SYS_REG_DBGBVR10_EL1;
  uint64_t HV_SYS_REG_DBGBCR10_EL1;
  uint64_t HV_SYS_REG_DBGBVR11_EL1;
  uint64_t HV_SYS_REG_DBGBCR11_EL1;
  uint64_t HV_SYS_REG_DBGBVR12_EL1;
  uint64_t HV_SYS_REG_DBGBCR12_EL1;
  uint64_t HV_SYS_REG_DBGBVR13_EL1;
  uint64_t HV_SYS_REG_DBGBCR13_EL1;
  uint64_t HV_SYS_REG_DBGBVR14_EL1;
  uint64_t HV_SYS_REG_DBGBCR14_EL1;
  uint64_t HV_SYS_REG_DBGBVR15_EL1;
  uint64_t HV_SYS_REG_DBGBCR15_EL1;
  uint64_t HV_SYS_REG_DBGWVR0_EL1;
  uint64_t HV_SYS_REG_DBGWCR0_EL1;
  uint64_t HV_SYS_REG_DBGWVR1_EL1;
  uint64_t HV_SYS_REG_DBGWCR1_EL1;
  uint64_t HV_SYS_REG_DBGWVR2_EL1;
  uint64_t HV_SYS_REG_DBGWCR2_EL1;
  uint64_t HV_SYS_REG_DBGWVR3_EL1;
  uint64_t HV_SYS_REG_DBGWCR3_EL1;
  uint64_t HV_SYS_REG_DBGWVR4_EL1;
  uint64_t HV_SYS_REG_DBGWCR4_EL1;
  uint64_t HV_SYS_REG_DBGWVR5_EL1;
  uint64_t HV_SYS_REG_DBGWCR5_EL1;
  uint64_t HV_SYS_REG_DBGWVR6_EL1;
  uint64_t HV_SYS_REG_DBGWCR6_EL1;
  uint64_t HV_SYS_REG_DBGWVR7_EL1;
  uint64_t HV_SYS_REG_DBGWCR7_EL1;
  uint64_t HV_SYS_REG_DBGWVR8_EL1;
  uint64_t HV_SYS_REG_DBGWCR8_EL1;
  uint64_t HV_SYS_REG_DBGWVR9_EL1;
  uint64_t HV_SYS_REG_DBGWCR9_EL1;
  uint64_t HV_SYS_REG_DBGWVR10_EL1;
  uint64_t HV_SYS_REG_DBGWCR10_EL1;
  uint64_t HV_SYS_REG_DBGWVR11_EL1;
  uint64_t HV_SYS_REG_DBGWCR11_EL1;
  uint64_t HV_SYS_REG_DBGWVR12_EL1;
  uint64_t HV_SYS_REG_DBGWCR12_EL1;
  uint64_t HV_SYS_REG_DBGWVR13_EL1;
  uint64_t HV_SYS_REG_DBGWCR13_EL1;
  uint64_t HV_SYS_REG_DBGWVR14_EL1;
  uint64_t HV_SYS_REG_DBGWCR14_EL1;
  uint64_t HV_SYS_REG_DBGWVR15_EL1;
  uint64_t HV_SYS_REG_DBGWCR15_EL1;
  uint64_t HV_SYS_REG_MDCCINT_EL1;
  char _p2[0x18];
  uint64_t control_field_0;
  uint64_t control_field_6;
  uint64_t control_field_1;
  uint64_t HV_SYS_REG_MDCR_EL2; // control_field_2, hv_vcpu_set_trap_debug_reg_accesses and hv_vcpu_set_trap_debug_exceptions
  uint64_t HV_SYS_REG_MPIDR_EL1; // control_field_3
  uint64_t HV_SYS_REG_MIDR_EL1; // control_field_5
  uint64_t HV_SYS_REG_CNTVOFF_EL2; // control_field_4
  uint64_t unknown0x6a0;
  uint64_t control_field_12;
  uint64_t control_field_13;
  uint64_t control_field_14;
  uint64_t control_field_15;
  uint64_t control_field_16;
  uint64_t vtimer_mask_reg; // hv_vcpu_set_vtimer_mask
  uint64_t control_field_7;
  uint64_t control_field_8;
  uint64_t control_field_9;
  uint64_t control_field_10;
  uint64_t control_field_11;
  uint64_t unknown0x700; // hv_vcpu_set_trap_debug_reg_accesses and hv_vcpu_set_trap_debug_exceptions related
  uint64_t exec_time; // the cumulative execution time of a vCPU, in nanoseconds.
  char _p3[0x60];
  uint64_t HV_SYS_REG_APGAKEYHI_EL1; // 0x770
  uint64_t HV_SYS_REG_APGAKEYLO_EL1;
  uint64_t HV_SYS_REG_APIAKEYHI_EL1;
  uint64_t HV_SYS_REG_APIAKEYLO_EL1;
  uint64_t HV_SYS_REG_APIBKEYHI_EL1;
  uint64_t HV_SYS_REG_APIBKEYLO_EL1;
  uint64_t HV_SYS_REG_APDAKEYHI_EL1;
  uint64_t HV_SYS_REG_APDAKEYLO_EL1;
  uint64_t HV_SYS_REG_APDBKEYHI_EL1; // 0x7b0
  uint64_t HV_SYS_REG_APDBKEYLO_EL1;
} *t_vcpu_context;

typedef struct vcpus {
  t_vcpu_context context;
  uint64_t HV_SYS_REG_ID_AA64DFR0_EL1;
  uint64_t HV_SYS_REG_ID_AA64DFR1_EL1;
  uint64_t HV_SYS_REG_ID_AA64ISAR0_EL1;
  uint64_t HV_SYS_REG_ID_AA64ISAR1_EL1;
  uint64_t HV_SYS_REG_ID_AA64MMFR0_EL1;
  uint64_t HV_SYS_REG_ID_AA64MMFR1_EL1;
  uint64_t HV_SYS_REG_ID_AA64MMFR2_EL1;
  uint64_t HV_SYS_REG_ID_AA64PFR0_EL1;
  uint64_t HV_SYS_REG_ID_AA64PFR1_EL1;
  char _p0[0x98];
  uint64_t HV_SYS_REG_HCR_EL2;
  char _p1[0x28];
} *t_vcpus;

extern "C" t_vcpu_context _hv_vcpu_get_context(hv_vcpu_t vcpu);

extern "C" hv_return_t _hv_vcpu_get_ext_reg(hv_vcpu_t vcpu, bool error, uint64_t *value);

extern "C" hv_return_t _hv_vcpu_set_control_field(hv_vcpu_t vcpu, int index, uint64_t value);

#define HY_VCPUS_OFFSET 0x1E286010

static t_vcpus lookupVcpu(hv_vcpu_t vcpu) {
  struct task_dyld_info dyld_info;
  mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
  task_t task = mach_task_self();
  assert(task_info(task, TASK_DYLD_INFO, (task_info_t)&dyld_info, &count) == 0);
  struct dyld_all_image_infos* infos = (struct dyld_all_image_infos*)(uintptr_t)dyld_info.all_image_info_addr;
  uint64_t hypervisor_load_address = 0;
  for(int i=0; i < infos->infoArrayCount; ++i) {
    const char *path = infos->infoArray[i].imageFilePath;
    if(strlen(path) > 0 && strstr(path, "/Hypervisor")) {
      hypervisor_load_address = (uint64_t) infos->infoArray[i].imageLoadAddress;
      break;
    }
  }
  if(hypervisor_load_address > 0) {
    t_vcpus vcpus = (t_vcpus) (hypervisor_load_address + HY_VCPUS_OFFSET);
    t_vcpus cpu = vcpus + vcpu;
    if(cpu->context == _hv_vcpu_get_context(vcpu)) {
      return cpu;
    }
  }
  return NULL;
}
